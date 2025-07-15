#ifndef TRACKING_FINDER_H
#define TRACKING_FINDER_H

#include <vector>
#include <memory>

#include "Algo/TypeDef.h"
#include "Algo/Object/seed.h"

#include "Algo/GreedyFinder/greedy_finder.h"

class WrappedFinder
{
private:
    using Finder = GreedyFinder;
    using Snapshot = Finder::GreedyFinderSnapshot;
    using Config = Finder::GreedyFinderConfig;
    using Seed_t = Seed<TrkHit>;
    using SeedContainer_t = std::vector<Seed_t>;
    using Pool_t = HitPool<Key, TrkHit>;
    using TrackContainer_t = std::vector<std::shared_ptr<DTrack>>;

public:
    WrappedFinder() { Init(); }
    ~WrappedFinder()
    {
        delete snapshot_;    snapshot_ = nullptr;
        delete track_finder_; track_finder_ = nullptr;
    }

    void Init()
    {
        delete snapshot_;
        snapshot_ = new Snapshot();

        delete track_finder_;
        track_finder_ = new Finder();
    }

    void Run(Config config, Pool_t *hit_pool, const SeedContainer_t &seeds)
    {
        track_finder_->Config(config);
        snapshot_->clear();
        track_finder_->FindTracks(hit_pool, snapshot_, seeds);
    }

    TrackContainer_t GetTracks() { return track_finder_->GetTracks<DTrack>(); }

private:
    Snapshot* snapshot_{nullptr};
    Finder *track_finder_{nullptr};
};

#endif // TRACKING_FINDER_H
