#ifndef TRACKING_SEEDING_FINDER_H
#define TRACKING_SEEDING_FINDER_H

#include "Algo/Utils/Util.h"
#include "Algo/Seeding/seed_helper.h"

#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>
#include <numeric>
//#include <ranges>
#include <cmath>

struct SeedFinderConfig
{
    double kasa_chi2 = 0.97;
    double linear_r2 = 0.03;
    double min_R = 250.;
    double sigma_z = 1.;
    size_t max_seed = 10;
    int verbose = 0;
};

template <typename seed_t, typename layers_t, typename hit_p_t>
class SeedFinder
{
private:
    using hit_t = tracking::utils::remove_pointer_s_t<hit_p_t>;
    using coordinate_getter = std::function<double(const hit_t&)>;

    struct Triplet
    {
        hit_p_t bottom_hit{nullptr};
        hit_p_t middle_hit{nullptr};
        hit_p_t top_hit{nullptr};

        double A;
        double B;
        double R;
    };


public:
    struct SeedFinderSnapshot
    {
        std::vector<seed_t> candidates;

        std::vector<double> seed_Rmax;
        std::vector<double> seed_chi2;
        std::vector<double> seed_r2;
        std::vector<int> seed_n_cand_per_mid;

        void Clear()
        {
            candidates.clear();
            seed_Rmax.clear();
            seed_chi2.clear();
            seed_r2.clear();
            seed_n_cand_per_mid.clear();
        }
    };

public:
    SeedFinder(const SeedFinderConfig &config) : config_(config) {}
    SeedFinder() {};
    SeedFinder(const SeedFinder<seed_t, layers_t, hit_t>&) = delete;

    ~SeedFinder() = default;

    void Connect(coordinate_getter get_x, coordinate_getter get_y, coordinate_getter get_z)
    {
        get_x_ = get_x;
        get_y_ = get_y;
        get_z_ = get_z;
    }

//................................................................................//
// Create seeds from the structured hit_t object, layers
// Assume seed_container_t and id_container_t are iterable and hits in layers are pointers/smart pointers;
//        layers_t has access to hit_t objects via at().at()
    template <typename seed_container_t, typename id_container_t>
    void CreateSeeds(SeedFinderSnapshot &snapshot,
                     const layers_t &layers,
                     seed_container_t &seeds,
                     const id_container_t &bottom_ids, const size_t &middle_id, const size_t &top_id)
    {
        using namespace tracking;

        static_assert(utils::is_iterable<id_container_t>::value, "id_container_t is not iterable in CreateSeeds()");
        static_assert(utils::is_iterable<seed_container_t>::value, "seed_container_t is not iterable in CreateSeeds()");

        using hit_p_in_layer_t = std::remove_cv_t<std::remove_reference_t<decltype(layers.at(0).at(0))>>;
        static_assert(std::is_same_v<hit_p_in_layer_t, hit_p_t>,
                      "Hit type in layers_t not compatible with hit_t in CreateSeeds()");
        using layer_t = std::remove_cv_t<std::remove_reference_t<decltype(layers.at(0))>>;
        static_assert(utils::is_iterable<layer_t>::value, "layer_t is not iterable in CreateSeeds()");

        using seed_in_cont_t  = std::remove_cv_t<std::remove_reference_t<decltype(seeds.at(0))>>;
        static_assert(std::is_same_v<seed_in_cont_t, seed_t>,
                      "Seed type in seed_container_t not compatible with seed_t in CreateSeeds()");

        seeds.clear();
        snapshot.Clear();

        if(bottom_ids.size() == 0 || bottom_ids.size() > 2)
        {
            std::cerr << "[WARNING] ==> Expect 1 or 2 bottom layers, " << bottom_ids.size() << " received" << std::endl;
            return;
        }     

        if(layers.size() < bottom_ids.size() + 1u + 1u)
            return;

        std::vector<hit_p_t> top_layer(layers.at(top_id).begin(), layers.at(top_id).end());
        std::vector<hit_p_t> middle_layer(layers.at(middle_id).begin(), layers.at(middle_id).end());

        std::vector<hit_p_t> bottom0_layer(layers.at(*bottom_ids.begin()).begin(), layers.at(*bottom_ids.begin()).end());
        std::vector<hit_p_t> bottom1_layer;
        if(bottom_ids.size() == 2) bottom1_layer.assign(layers.at(*bottom_ids.rbegin()).begin(), layers.at(*bottom_ids.rbegin()).end());

        auto get_t = [](double x1, double y1, double x2, double y2, double a, double b)
                     {
                         double a_dot_b = (x1 - a)*(x2 - a) + (y1 - b)*(y2 - b);
                         double r1sq = (x1 - a)*(x1 - a) + (y1 - b)*(y1 - b);
                         double r2sq = (x2 - a)*(x2 - a) + (y2 - b)*(y2 - b);
                         return std::acos(a_dot_b/sqrt(r1sq*r2sq));
                     };

        auto get_r = [](double x1, double y1, double x2, double y2)
                     {
                         return std::hypot(x1 - x2, y1 - y2);
                     };

        auto print_quadruplet = [](const auto &hit1, const auto &hit2, const auto &hit3, const auto &hit4)
                                {
                                    std::cout << hit1 << std::endl << hit2 << std::endl
                                              << hit3 << std::endl << hit4 << std::endl;
                                };

        snapshot.candidates.reserve(top_layer.size()*middle_layer.size()*bottom0_layer.size()*bottom1_layer.size());
        for(auto &middle_hit : middle_layer)
        {
            if(config_.verbose > 0) std::cout << "\nmiddle: " << *middle_hit << "\n" << std::endl;

            std::vector<Triplet> triplet_candidates;
            triplet_candidates.reserve(top_layer.size()*bottom0_layer.size()*bottom1_layer.size());

            SelectTripletCandidate(triplet_candidates, bottom0_layer, bottom1_layer, top_layer);

            snapshot.seed_Rmax.push_back(0.);
            int n_cand_per_mid(0);
            for(const auto &triplet : triplet_candidates)
            {
                double x[] = {get_x_(*middle_hit), get_x_(*triplet.top_hit), get_x_(*triplet.middle_hit), get_x_(*triplet.bottom_hit)};
                double y[] = {get_y_(*middle_hit), get_y_(*triplet.top_hit), get_y_(*triplet.middle_hit), get_y_(*triplet.bottom_hit)};
                double z0[] = {get_z_(*middle_hit), get_z_(*triplet.top_hit), get_z_(*triplet.middle_hit), get_z_(*triplet.bottom_hit)};
                double z[] = {0., z0[1] - z0[0], z0[2] - z0[0], z0[3] - z0[0]};

                double A;
                double B;
                double R;
                double chi2;
                seed_helper::KasaFit(x, y, 4, A, B, R, chi2);
//                std::cout << A << "\t" << B << "\t " << R << "\t" << chi2 << std::endl;

                double r1 = get_r(x[1], x[0], y[1], y[0]);
                double r2 = get_r(x[2], x[0], y[2], y[0]);
                double r3 = get_r(x[3], x[0], y[3], y[0]);
                double r[] = {0., r1, r2, r3};
//                std::cout << t[0] << "\t" << t[1] << "\t "<< t[2] << std::endl;

                double abr[3];
                seed_helper::LinearFit(z, r, 4, abr);
//                std::cout << abr[0] << "\t" << abr[1] << "\t "<< abr[2] << std::endl;
//                std::cout << std::endl;

                snapshot.seed_chi2.push_back(chi2);
                snapshot.seed_r2.push_back(abr[2]);
//                std::cout << *triplet.top_hit << ", \tR:" << R << ", \tchi2: " << chi2 << ",\tr2: " << abr[2] << std::endl;

                auto R_change = std::abs(std::hypot(x[0] - triplet.A, y[0] - triplet.B) - triplet.R);
                if(R_change > 2.5)
                {
                    if(config_.verbose > 0)
                    {
                        std::cout << "rejected R change:" << R_change << "\n";
                        print_quadruplet(*triplet.bottom_hit, *triplet.middle_hit, *middle_hit, *triplet.top_hit);
                    }
                    continue;
                }
                if(chi2 < config_.kasa_chi2 || abr[2] < config_.linear_r2)
                {
                    if(config_.verbose > 0)
                    {
                        std::cout << "rejected chi2: " << chi2 << " or r2: " << abr[2] << "\n";
                        print_quadruplet(*triplet.bottom_hit, *triplet.middle_hit, *middle_hit, *triplet.top_hit);
                    }                  
                    continue;
                }

                std::vector<hit_p_t> bottoms = {triplet.bottom_hit, triplet.middle_hit};
                snapshot.candidates.emplace_back(bottoms, middle_hit, triplet.top_hit, A, B, R, chi2, abr[2]);

                if(R > snapshot.seed_Rmax.back()) snapshot.seed_Rmax.back() = R;
                n_cand_per_mid++;
            }

            snapshot.seed_n_cand_per_mid.push_back(n_cand_per_mid);
        }

        FilterSeeds(snapshot, seeds);

        if(config_.verbose > 0)
        {
            std::cout << "seed:\n" << std::endl;
            for(const auto &seed : seeds) std::cout << seed << std::endl;
            std::cout << std::endl;
        }
    }

private:
    void SelectTripletCandidate(std::vector<Triplet> &candidates,
                                std::vector<hit_p_t> &bottom_layer, std::vector<hit_p_t> &middle_layer,
                                std::vector<hit_p_t> &top_layer)
    {
        if(bottom_layer.size() == 0)
        {
            return;
        }

        for(auto &bottom_hit : bottom_layer)
        {
//            std::cout << *bottom_hit << "\n" << std::endl;
            for(auto &middle_hit : middle_layer)
            {
//                std::cout << *middle_hit << "\n" << std::endl;
                for(auto &top_hit : top_layer)
                {
//                    std::cout << *top_hit << "\n" << std::endl;
                    double x[] = {get_x_(*top_hit), get_x_(*middle_hit), get_x_(*bottom_hit)};
                    double y[] = {get_y_(*top_hit), get_y_(*middle_hit), get_y_(*bottom_hit)};

                    double A;
                    double B;
                    double R;
                    double chi2;
                    seed_helper::KasaFit(x, y, 3, A, B, R, chi2);
//                    std::cout << "R: " << R << std::endl;

                    if(R < config_.min_R*1.2)
                    {
                        if(config_.verbose > 0)
                        {
                            std::cout << "rejected R:" << R << "\n";
                            std::cout << *bottom_hit << std::endl
                                      << *middle_hit << std::endl
                                      << *top_hit << std::endl;
                        }
                        continue;
                    }

                    candidates.emplace_back(bottom_hit, middle_hit, top_hit, A, B, R);
//                    std::cout << *top_hit << ", \tchi2: " << chi2 << ", R: " << R << std::endl;
                }
            }
        }
    }

    template <typename seed_container_t>
    void FilterSeeds(SeedFinderSnapshot &snapshot, seed_container_t &seeds)
    {
        using namespace tracking;

        if(config_.verbose > 0)
        {
            std::cout << "\ncandidate:\n" << std::endl;
            for(const auto &seed : snapshot.candidates) std::cout << seed << std::endl;
        }

        int seed_n(0);
        for(auto &&[seed_n_i, Rmax] : utils::make_zip(snapshot.seed_n_cand_per_mid, snapshot.seed_Rmax))
        {
            std::sort(snapshot.candidates.begin() + seed_n,
                      snapshot.candidates.begin() + seed_n + seed_n_i,
                      [=](const seed_t &s1, const seed_t &s2)
                      {
                          return std::hypot(s1.GetQ1(), s1.GetQ2(), s1.GetR0()/Rmax) >
                                 std::hypot(s2.GetQ1(), s2.GetQ2(), s2.GetR0()/Rmax);
                      }
                     );
            auto max_seed = (config_.max_seed < seed_n_i) ? config_.max_seed : seed_n_i;
            seeds.insert(seeds.end(),
                         snapshot.candidates.begin() + seed_n,
                         snapshot.candidates.begin() + seed_n + max_seed);

            seed_n += seed_n_i;
        }
    }

private:
    SeedFinderConfig config_;

    coordinate_getter get_x_;
    coordinate_getter get_y_;
    coordinate_getter get_z_;

    std::vector<double> seed_chi2_;
    std::vector<double> seed_r2_;
    std::vector<int> seed_n_cand_per_mid_;
};

#endif // SEED_FINDER_H
