#ifndef TRACKING_GREEDY_FINDER_H
#define TRACKING_GREEDY_FINDER_H

#include <memory>
#include <map>
#include <vector>
#include <functional>

#include "Algo/Object/TrkHit.h"
#include "Algo/Object/seed.h"
#include "Algo/Object/HitPool.h"

class GreedyFinder
{
private:
    using hit_p_t = std::shared_ptr<TrkHit>; 
    using seed_t = Seed<TrkHit>;
    using pool_t = HitPool<Key, TrkHit>;
    using hit_map_t = std::map<int, std::vector<hit_p_t>>;

public:
    struct GreedyFinderConfig
    {
        int max_circle = 100000;
        size_t min_depth = 3;
        double min_R = 250.;
        double max_R = 27000.;
        double kasa_chi2 = 0.9995;
        double linear_r2 = 0.9;
        int verbose = 0;
    };

    struct GreedyFinderSnapshot
    {
        int track_n{0};
        std::vector<double> track_chi2;
        std::vector<double> track_r2;

        void clear()
        {
            track_n = 0;
        
            track_chi2.clear();
            track_r2.clear();
        }
    };

private:
    struct ProtoTrack
    {
        std::vector<hit_p_t> candidate;
        std::vector<double>  x;
        std::vector<double>  y;
        std::vector<double>  z;

        std::vector<int> seed_ids;

        void clear();
        void push_back(const hit_p_t &hit);
        void pop_back();
        bool IsSeeded(int id) {return std::find(seed_ids.begin(), seed_ids.end(), std::abs(id)) != seed_ids.end();}

        ProtoTrack& operator =(const seed_t &seed);
    };

    struct ProtoTrackManager
    {
        double best_chi2{0.};
        double best_r2{0.};

        std::shared_ptr<ProtoTrack> current_track{nullptr};
        double current_chi2{0.};
        double current_r2{0.};
        double current_A{0.};
        double current_B{0.};
        double current_R{0.};

        std::shared_ptr<ProtoTrack> chosen_track{nullptr};
        double chosen_chi2{0.};
        double chosen_r2{0.};
        double chosen_A{0.};
        double chosen_B{0.};
        double chosen_R{0.};

        std::vector<std::shared_ptr<ProtoTrack>> output_tracks;
        std::vector<double> output_chi2s;
        std::vector<double> output_r2s;
        std::vector<double> output_As;
        std::vector<double> output_Bs;
        std::vector<double> output_Rs;

        void reset(const GreedyFinderConfig &config, double *chi2 = nullptr, double *r2 = nullptr);
        void clear(const GreedyFinderConfig &config, double *chi2 = nullptr, double *r2 = nullptr);
        void Choose();
        template<class id_container_t> void Remove(const id_container_t &removed_ids);
        void Output();
        void SortTrackHits();
    };

public:
    GreedyFinder() {}

    GreedyFinder(const GreedyFinderConfig &config) : config_(config) {}

    void FindTracks(pool_t *pool, GreedyFinderSnapshot *snapshot, const std::vector<seed_t> &seeds = {});

    template<class track_t>
    std::vector<std::shared_ptr<track_t>> GetTracks()
    {
        std::vector<std::shared_ptr<track_t>> tracks;
        tracks.reserve(manager_.output_tracks.size());

        auto i_track = 0;
        for(const auto &track : manager_.output_tracks)
        {
            tracks.push_back(std::make_shared<track_t>(track->candidate,
                                                       manager_.output_Rs.at(i_track),
                                                       manager_.output_As.at(i_track), manager_.output_Bs.at(i_track)
//                                                       manager_.output_chi2s.at(i_track), manager_.output_r2s.at(i_track))
                                                      ));

//            for(const auto &hit : track->candidate)
//                hit->SetTrack(tracks.back());

            i_track++;
        }

        return tracks;
    }

    void Config(GreedyFinderConfig config) {config_ = config;};

private:
    void KasaFit();
    void ColinearFit();

    hit_map_t GetFurnishedPool(pool_t *pool);
    void ShrinkPool(hit_map_t &pool);

    void RejectTrack();
    void MergeTrack();
    bool GreedyLooping(hit_map_t &pool, hit_map_t::iterator layer, const seed_t *seed = nullptr);

    GreedyFinderConfig config_;
    GreedyFinderSnapshot *snapshot_{nullptr};

    ProtoTrackManager manager_;

    int circle_No_{0};
};

#include "proto_track.ipp"

#endif // TRACKING_GREEDY_FINDER_H
