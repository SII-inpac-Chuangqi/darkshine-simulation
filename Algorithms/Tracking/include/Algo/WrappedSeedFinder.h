#ifndef TRACKING_SEED_FINDER_H
#define TRACKING_SEED_FINDER_H

#include <vector>

#include "Algo/TypeDef.h"
#include "Algo/Object/seed.h"
#include "Algo/Object/HitPool.h"
#include "Algo/Seeding/seed_finder.h"

class WrappedSeedFinder
{
private:
    using Seed_t = Seed<TrkHit>;
    using SeedContainer_t = std::vector<Seed_t>;
    using Finder = SeedFinder<Seed_t, TrkHitSPVecMap, TrkHitSP>;
    using Snapshot = Finder::SeedFinderSnapshot;
    using Pool_t = HitPool<Key, TrkHit>;
    using Config_t = Finder::SeedFinderConfig;
    using CoordinateGetter_t_t = std::function<double(const TrkHit&)>;

public:
    using SeedFinderConfig = Finder::SeedFinderConfig;

public:
    WrappedSeedFinder() { Init(); };
    ~WrappedSeedFinder()
    {
        delete snapshot_;    snapshot_ = nullptr;
        delete seed_finder_; seed_finder_ = nullptr;
    }

    void Init()
    {
        delete snapshot_;
        snapshot_ = new Snapshot();

        delete seed_finder_;
        seed_finder_ = new Finder();
    }

    void Connect(CoordinateGetter_t_t get_x, CoordinateGetter_t_t get_y, CoordinateGetter_t_t get_z)
    {
        if(!seed_finder_) return;
        seed_finder_->Connect(get_x, get_y, get_z);
    }

    void Run(Config_t config, SeedContainer_t &seeds, Pool_t *pool)
    {
        if(!snapshot_ || !seed_finder_ || !pool) return;

        seed_finder_->Config(config);
        snapshot_->clear();

        int min_seed_size = config.bottoms_n + 1 + 1;

        auto [bottom_ids, middle_id, top_id] = pool->GetIds(config.bottoms_n);
/*
        auto print_ids = [](const auto &ids)
                         {
                             TString output;
                             int i_id = ids.size() - 1;
                             for(const auto &id : ids)
                                 output += TString::Format("%i%s", id, (i_id-- == 0 ? "" : ", "));
                             return output;
                         };

        std::cout << "bottom ids: " << print_ids(bottom_ids)
                  << " middle id: "  << middle_id << " top id: " << top_id << std::endl;
*/

        seed_finder_->FindSeeds(*snapshot_, *pool->GetPool(), seeds, bottom_ids, middle_id, top_id);
//        std::cout << seeds.size() << " seeds are found" << std::endl;

        if(seeds.size() == 0 && static_cast<int>(pool->size()) >= min_seed_size)
        {
            std::vector<int> shifts;
            for(int shift = 1; shift <= static_cast<int>(pool->size()) - min_seed_size; shift++)
            { shifts.push_back(shift); shifts.push_back(-shift); }

            for(const auto shift : shifts)
            {
               std::tie(bottom_ids, middle_id, top_id) = pool->GetIds(config.bottoms_n, shift);
               seed_finder_->FindSeeds(*snapshot_, *pool->GetPool(), seeds, bottom_ids, middle_id, top_id);
               if(seeds.size() > 0) break;
            }
        }
    }

private:
    Snapshot *snapshot_{nullptr};
    Finder *seed_finder_{nullptr};
};

#endif // TRACKING_SEED_FINDER_H

