//
// Created by Yulei on 2023/6/22.
//

#include "Algorithms/DataExporter/include/Algo/GNN_Digitization.h"

#include "Core/AnaData.h"


SimulatedHitVec
GNN_Digitization::Run(
        SimulatedHitVec *track1,
        SimulatedHitVec *track2,
        bool if_tag_or_rec,
        float min_energy
) {
    // initialize geometry parameters for tagging tracker and recoil tracker
    auto trk_angles = if_tag_or_rec ? dAnaData->getAnglesTag() : dAnaData->getAnglesRec();
    auto trk_region = if_tag_or_rec ? dAnaData->getLayerLengthTag() : dAnaData->getLayerLengthRec();
    auto trk_width = if_tag_or_rec ? dAnaData->getLayerWidthTag() : dAnaData->getLayerWidthRec();
    auto trk_strip_no = if_tag_or_rec ? dAnaData->getStripNoTag() : dAnaData->getStripNoRec();

    // Combine the two tracks into a single vector, keyed by layer
    std::map<uint16_t, std::pair<SimulatedHitVec, SimulatedHitVec>> trackMapByLayer;

    auto insert_hit = [&trackMapByLayer](SimulatedHitVec *track, bool first_track = true) {
        for (auto hit: *track) {
            // If the layer is not in the map, add it
            trackMapByLayer.try_emplace(hit->getCellIdZ(), std::make_pair(SimulatedHitVec(), SimulatedHitVec()));

            // Add the hit to the map
            (first_track) ?
            trackMapByLayer[hit->getCellIdZ()].first.push_back(hit) :
            trackMapByLayer[hit->getCellIdZ()].second.push_back(hit);
        }
    };

    insert_hit(track1, true);
    insert_hit(track2, false);

    // Perform mean shift clustering on each layer
    SimulatedHitVec final_hits;
    for (const auto &hits_layer: trackMapByLayer) {
        auto layer_id = hits_layer.first - 1;
        auto layer_range = trk_region[layer_id * 2];
        auto layer_angle = trk_angles[layer_id * 2 + 1];
        auto layer_band = static_cast<float>(trk_width[layer_id * 2 + 1] / trk_strip_no[layer_id * 2 + 1]);

        auto process = [layer_band, min_energy](const SimulatedHitVec &hits) {
            // Convert the hits to points
            std::vector<Point3D> points;
            points.reserve(hits.size());
            for (auto hit: hits) {
                auto point = Point3D(hit->getX(), hit->getY(), hit->getZ());
                point.hit = hit;
                points.emplace_back(point);
            }

            // Perform mean shift clustering
            auto newPoints = meanShift(points, 3 * layer_band, static_cast<float>(0.1 * layer_band));

            // Convert the points back to hits
            auto newHits = cluster(newPoints, 2 * layer_band, min_energy);

            return newHits;
        };

        auto hit_track1 = process(hits_layer.second.first);
        auto hit_track2 = process(hits_layer.second.second);

        /*
         * Track 1 is always along y-axis
         * Track 2 has a small angle (+/- 0.1 rad) with respect to the y-axis
         * To calculate the intersection of track 1 and track 2:
         *   -- For each hit in track 1, calculate the intersection with each hit in track 2
         *   -- expr of tracker 2: y = tan(theta) * (x - x2)
         *   -- expr of intersection: y = tan(theta) * (x1 - x2)
         *   -- The intersection mush be within the range of tracker region
         */

        for (auto hit1: hit_track1) {
            for (auto hit2: hit_track2) {
                auto x1 = hit1->getX();
                auto x2 = hit2->getX();
                auto y2 = hit2->getY();

                auto x = x1;
                auto y = TMath::Tan(layer_angle) * (x1 - x2) + y2;

                if (y < 0.5 * layer_range && y >= -0.5 * layer_range) {
                    auto *hit = new SimulatedHit();
                    hit->setX(x);
                    hit->setY(static_cast<float>(y));
                    hit->setZ(static_cast<float>(0.5 * (hit1->getZ() + hit2->getZ())));
                    hit->setCellIdZ(hit1->getCellIdZ());

                    /*
                     * Merge the MC contribution
                     *  -- Sort the MC particles by id
                     *  -- If there are multiple MC particles with the same id, select the one with the lowest energy
                     *  -- Only keeps the duplicates (duplicates have been already removed in the clustering process)
                     *
                     *  There are two types of digitized hits (layer 1 + layer 2):
                     *  1. (w. MCContribution) The hit is from the same MC particle as the original hit
                     *  2. (w.o. MCContribution) The hit is from a different MC particle (typically low energy) or ambiguity
                     */
                    std::vector<McParticle> mcps;
                    mcps.reserve(hit1->getPContribution().size() + hit2->getPContribution().size());
                    std::copy(
                            hit1->getPContribution().begin(),
                            hit1->getPContribution().end(),
                            std::back_inserter(mcps)
                    );
                    std::copy(
                            hit2->getPContribution().begin(),
                            hit2->getPContribution().end(),
                            std::back_inserter(mcps)
                    );

                    std::sort(mcps.begin(), mcps.end(), [](const McParticle &a, const McParticle &b) {
                        if (a.getId() == b.getId()) {
                            return a.getEnergy() < b.getEnergy();  // within the same id group, sort by E ascending
                        } else {
                            return a.getId() < b.getId();  // sort primarily by id ascending
                        }
                    });

                    std::vector<McParticle> duplicates;
                    auto it = mcps.begin();
                    while (it != mcps.end()) {
                        it = std::adjacent_find(it, mcps.end(), [](const McParticle &a, const McParticle &b) {
                            return a.getId() == b.getId();
                        });
                        if (it != mcps.end()) {
                            duplicates.push_back(*it);
                            // Skip over remaining elements with the same id
                            int current_id = it->getId();
                            while (it != mcps.end() && it->getId() == current_id) {
                                ++it;
                            }
                        }
                    }
                    mcps = std::move(duplicates);

                    hit->setPContribution(mcps);

                    final_hits.push_back(hit);
                }

            }
        }
    }

    return final_hits;
}

/**
 * Perform mean shift clustering on the given points
 * @param points The points to cluster
 * @param band_width The band width
 * @param epsilon The convergence threshold
 * @return The shifted points
 */
std::vector<Point3D> GNN_Digitization::meanShift(const std::vector<Point3D> &points, float band_width, float epsilon) {
    // Make a copy of the original points
    std::vector<Point3D> oldPoints(points);
    // Allocate space for the new points
    std::vector<Point3D> newPoints(points.size());

    bool hasConverged = false;

    // Repeat until convergence
    while (!hasConverged) {
        hasConverged = true;

        // std::transform applies a function to each element in the range [oldPoints.begin(), oldPoints.end()) and
        // stores the result in newPoints. The third argument to std::transform is the start of the destination range.
        std::transform(
                oldPoints.begin(), oldPoints.end(), newPoints.begin(),
                [&](const Point3D &oldPoint) {
                    // std::accumulate computes the sum of values in a range [oldPoints.begin(), oldPoints.end()).
                    // The fourth argument is a binary function that accepts two values and produces a result.
                    // In this case, the binary function takes an accumulated value (shift, count) and a point,
                    // and returns a new accumulated value.
                    auto [shift, count] = std::accumulate(
                            oldPoints.begin(), oldPoints.end(),
                            std::pair<Point3D, int>{},
                            [&](const auto &acc, const Point3D &point) {
                                auto [shift_, count_] = acc;
                                float distance = oldPoint.distanceTo(point);
                                if (distance < band_width) {
                                    shift_ += point;
                                    count_++;
                                }
                                // Return the new accumulated value
                                return std::pair<Point3D, int>{shift_, count_};
                            }
                    );

                    // Compute the new point as the average of the shift
                    Point3D newPoint = shift / static_cast<float>(count);
                    newPoint.hit = oldPoint.hit;

                    // Check if the new point is close enough to the old point
                    if (newPoint.distanceTo(oldPoint) > epsilon) {
                        hasConverged = false;
                    }

                    // Return the new point, which std::transform will store in newPoints
                    return newPoint;
                }
        );

        // Swap oldPoints and newPoints for the next iteration
        oldPoints.swap(newPoints);
    }

    // Return the final points after convergence
    return oldPoints;
}

/**
 * Cluster the given points using mean shift clustering
 * @param points The points to cluster
 * @param tolerance The tolerance
 * @param min_energy The minimum energy for MCContributions
 * @return The cluster centers
 */
SimulatedHitVec GNN_Digitization::cluster(const vector<Point3D> &points, float tolerance, float min_energy) {
    // cluster mean-shifted points
    std::map<unsigned int, std::vector<Point3D>> clusters;

    for (const auto &point: points) {
        // use std::find_if to find the first cluster within tolerance
        auto it = std::find_if(
                clusters.begin(), clusters.end(),
                [&](const auto &cluster) {
                    return point.distanceTo(cluster.second[0]) < tolerance;
                }
        );

        // if a cluster is found within tolerance, add the point to this cluster
        if (it != clusters.end()) {
            it->second.push_back(point);
        } else {
            // else create a new cluster
            clusters.emplace(clusters.size(), std::vector<Point3D>{point});
        }
    }

    // return the cluster centers
    SimulatedHitVec new_hits;
    for (const auto &cluster: clusters) {
        auto *hit = new SimulatedHit();

        auto center = std::accumulate(
                cluster.second.begin(), cluster.second.end(),
                Point3D{0, 0, 0},
                [](const auto &acc, const Point3D &point) {
                    return acc + point;
                }
        ) / static_cast<float>(cluster.second.size());

        hit->setX(center.x);
        hit->setY(center.y);
        hit->setZ(center.z);
        hit->setCellIdZ(cluster.second.front().hit->getCellIdZ());

        std::vector<McParticle> mcps;
        for (const auto &point: cluster.second) {
            // record the mc particles that contributed to this hit
            std::copy(
                    point.hit->getPContribution().begin(),
                    point.hit->getPContribution().end(),
                    std::back_inserter(mcps)
            );
        }

        // Remove duplicate particles with same id, only keep the smallest Energy
        std::sort(mcps.begin(), mcps.end(), [](const McParticle &a, const McParticle &b) {
            if (a.getId() == b.getId()) {
                return a.getEnergy() < b.getEnergy();  // within the same id group, sort by E ascending
            } else {
                return a.getId() < b.getId();  // sort primarily by id ascending
            }
        });


        // remove consecutive (adjacent) duplicates
        auto last = std::unique(mcps.begin(), mcps.end(), [](const McParticle &a, const McParticle &b) {
            return a.getId() == b.getId();
        });
        mcps.erase(last, mcps.end());


        // Remove all elements where Energy < min_energy
        mcps.erase(
                std::remove_if(
                        mcps.begin(), mcps.end(),
                        [min_energy](const McParticle &mcp) { return mcp.getEnergy() < min_energy; }),
                mcps.end()
        );


        hit->setPContribution(mcps);

        new_hits.push_back(hit);
    }

    return new_hits;
}
