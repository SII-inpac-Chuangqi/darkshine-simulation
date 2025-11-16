#include "Algo/GreedyFinder/greedy_finder.h"

#include <algorithm>
#include <set>

#include "Algo/Seeding/seed_helper.h"

void GreedyFinder::KasaFit()
{
    auto n_hit = manager_.current_track->candidate.size();
    if(n_hit < 3) return;

    seed_helper::KasaFit(manager_.current_track->x.data(), manager_.current_track->z.data(), n_hit,
                         manager_.current_A, manager_.current_B, manager_.current_R, manager_.current_chi2);
}

void GreedyFinder::ColinearFit()
{
    auto n_hit = manager_.current_track->candidate.size();
    if(n_hit < 3) return;

    auto r = manager_.current_track->x;
    auto x0 = manager_.current_track->x.at(0);
    auto y0 = manager_.current_track->y.at(0);
    std::transform(manager_.current_track->x.begin(), manager_.current_track->x.end(), manager_.current_track->y.begin(), r.begin(),
                   [=](double xi, double yi) { return std::hypot(xi - x0, yi - y0); });

    double abr[3];
    seed_helper::LinearFit(manager_.current_track->z.data(), r.data(), n_hit, abr);
    manager_.current_r2 = abr[2];
}

GreedyFinder::hit_map_t GreedyFinder::GetFurnishedPool(pool_t *pool)
{
    if(pool->IsNull()) return {{}};
    if(pool->size() == 0)
    {
        std::cerr << "[WARNING] ==> Empty pool passed to track finder" << std::endl;
        return {{}};
    }

    auto furnished_pool = *pool->GetPool();

    if(furnished_pool.size() <= 2) return furnished_pool;

    int *layers = new int[furnished_pool.size() - 2];

    size_t i = 0;
    for(const auto &layer : furnished_pool)
     {
        if(layer.first != furnished_pool.begin() ->first &&
           layer.first != furnished_pool.rbegin()->first)
        {
            layers[i] = layer.first;
            i++;
        }
     }

     for(i = 0; i < furnished_pool.size() - 2; i++)
     {
         auto layer = furnished_pool.extract(layers[i]);
         layer.key() = -layer.key();
         furnished_pool.insert(std::move(layer));
     }

    delete [] layers;

    return furnished_pool;
}

void GreedyFinder::ShrinkPool(hit_map_t &pool)
{
    for(auto layer = pool.begin(); layer != pool.end();)
    {
        auto &hits = layer->second;

        hits.erase(std::remove_if(hits.begin(), hits.end(),
                                 [](const auto &hit) { return hit->Tracked(); }),
                   hits.end());

        if (hits.empty())
            layer = pool.erase(layer);
        else ++layer;
    }
}

void GreedyFinder::FindTracks(pool_t *pool, GreedyFinderSnapshot *snapshot, const std::vector<seed_t> &seeds)
{
    circle_No_ = 0;

    snapshot_ = snapshot;

    manager_.clear(config_);

    auto furnished_pool = GetFurnishedPool(pool);

    if(config_.verbose > 1)
    {
        for(const auto &[layer_no, hits] : furnished_pool)
        {
            std::cout << "layer: " << layer_no << std::endl;
            for(const auto &hit : hits)
                std::cout << *hit << std::endl;
        }
        std::cout << std::endl;
    }

    if(furnished_pool.size() == 0) return;

    auto layer = furnished_pool.end();

    int n_seed = 0;
    for(const auto &seed : seeds)
    {
        manager_.reset(config_);

        if(config_.verbose > 0) std::cout << "n seed: " << n_seed++ << std::endl;

        *manager_.current_track = seed;
        if(!GreedyLooping(furnished_pool, layer, &seed)) continue;

        manager_.Output();
    }

    MergeTrack();

    if(config_.verbose > 0)
        std::cout << manager_.output_tracks.size() << " tracks are found at 1st round\n" << std::endl;

    double refind_chi2 = 0.99;
    double refind_r2 = 0.60;
    manager_.reset(config_, &refind_chi2, &refind_r2);

    ShrinkPool(furnished_pool);

    while(furnished_pool.size() >= 3)
    {
        layer = furnished_pool.end();

        if(GreedyLooping(furnished_pool, layer) && manager_.chosen_track != nullptr)
        {
            manager_.Output();
            MergeTrack();
        }
        else break;

        ShrinkPool(furnished_pool);
    }

    manager_.SortTrackHits();

    snapshot_->track_n = manager_.output_tracks.size();

    if(config_.verbose > 0)
        std::cout << manager_.output_tracks.size() << " tracks are found finally" << std::endl;
}

void GreedyFinder::RejectTrack()
{
    if(manager_.current_track->candidate.size() <= config_.min_depth)
        return;

    KasaFit();

    ColinearFit();

    snapshot_->track_chi2.push_back(manager_.current_chi2);
    snapshot_->track_r2.push_back(manager_.current_r2);

    if(config_.verbose > 0)
    {
        for(const auto &hit : manager_.current_track->candidate)
            std::cout << *hit << std::endl;
        std::cout << "best chi2: " << manager_.best_chi2 << std::endl;
        std::cout << "best r2:   " << manager_.best_r2 << std::endl;
        std::cout << "min R:     " << config_.min_R << std::endl;
        std::cout << "chi2: " << manager_.current_chi2 << std::endl;
        std::cout << "r2:   " << manager_.current_r2 << std::endl;
        std::cout << "R:    " << manager_.current_R << std::endl;
        std::cout << std::endl;
    }

    if(manager_.current_chi2 > manager_.best_chi2 && manager_.current_r2 > manager_.best_r2 &&
       manager_.current_R > config_.min_R && manager_.current_R < config_.max_R)
    {
        manager_.best_chi2 = manager_.current_chi2;
        if(config_.verbose > 2)
            std::cout << "choose chi2 " << manager_.current_chi2 << " and r2: " << manager_.current_r2 << std::endl;
        manager_.Choose();
    }
}

void GreedyFinder::MergeTrack()
{
    auto &output_tracks = manager_.output_tracks;
    auto pre_size = output_tracks.size();
    if(pre_size < 1) return;

    std::set<int> track_to_rm;

    for(size_t i = 0; i < output_tracks.size(); i++)
    {
        const std::vector<hit_p_t> &track_i = output_tracks.at(i)->candidate;
        for(size_t j = i + 1; j < output_tracks.size(); j++)
        {
            double err_A = manager_.output_As.at(i) - manager_.output_As.at(j);
            double err_B = manager_.output_Bs.at(i) - manager_.output_Bs.at(j);
            double err_R = (manager_.output_Rs.at(i) - manager_.output_Rs.at(j))/manager_.output_Rs.at(j);
//            if(std::abs(err_A) < 10. && std::abs(err_B) < 10. && std::abs(err_R) < 0.01)
//                std::cout << "track i-j = A: " << err_A << " B: " << err_B << " R: " << err_R << std::endl;

            const std::vector<hit_p_t> &track_j = manager_.output_tracks.at(j)->candidate;
            auto c = std::count_if(track_i.begin(), track_i.end(),
                                   [=](const hit_p_t &hit)
                                   { return std::find(track_j.begin(), track_j.end(), hit) != track_j.end(); });
            if(c >= 3 ||
               (c == 2 && std::abs(std::hypot(err_A, err_B))/manager_.output_Rs.at(i) < 0.01 && std::abs(err_R) < 0.01))
            {
                if(config_.verbose > 1)
                    std::cout << "track " << i << "-" << j << " share " << c << " hits,"
                              << " and diff in A: " << err_A << " B: " << err_B << " R: " << err_R
                              << " chi2 i: " << manager_.output_chi2s.at(i)
                              << " chi2 j: " << manager_.output_chi2s.at(j) << std::endl;

                if(manager_.output_chi2s.at(i) > manager_.output_chi2s.at(j))
                    track_to_rm.insert(j);
                else track_to_rm.insert(i);
            }
        }
    }

    manager_.Remove(track_to_rm);

    int i_track = 0;
    for(auto &track : output_tracks)
    {
        for(auto &hit : track->candidate)
            hit->SetTracked();

        if(config_.verbose <= 1) continue;
        std::cout << "output track " << i_track << ":" << std::endl;
        for(auto &hit : track->candidate)
            std::cout << *hit << std::endl;
    }

    if(config_.verbose > 0 && pre_size != output_tracks.size())
        std::cout << pre_size - output_tracks.size() << " tracks are removed, current track count: "
                  << output_tracks.size() << std::endl;
}

bool GreedyFinder::GreedyLooping(hit_map_t &pool, hit_map_t::iterator layer, const seed_t *seed)
{
    if(circle_No_++ >= config_.max_circle) return false;

    layer--;
    const auto &layer_id   = layer->first;
    const auto &layer_hits = layer->second;

    auto is_seeded = (seed != nullptr) ? manager_.current_track->IsSeeded(layer_id) : false;
    if(config_.verbose > 2)
        std::cout << "layer: " << layer_id << (is_seeded ? " is seeded" : " is not seeded") << std::endl;

    if(layer == pool.begin())
    {
        if(is_seeded)
        {
            RejectTrack();

            return true;
        }

        for(size_t hit_no = 0; hit_no < layer_hits.size(); hit_no++)
        {
            manager_.current_track->push_back(layer_hits.at(hit_no));

            RejectTrack();

            manager_.current_track->pop_back();
        }

        return true;
    }

    if(is_seeded)
    {
        if(!GreedyLooping(pool, layer, seed))
            return false;
        return true;
    }

    for(size_t hit_no = 0; hit_no < layer_hits.size(); hit_no++)
    {
        manager_.current_track->push_back(layer_hits.at(hit_no));

        if(!GreedyLooping(pool, layer, seed))
        {
            manager_.current_track->pop_back();
            return false;
        }

        manager_.current_track->pop_back();
    }

    return true;
}
