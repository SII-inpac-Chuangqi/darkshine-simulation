inline void GreedyFinder::ProtoTrack::clear()
{
    candidate.clear();
    x.clear();
    y.clear();
    z.clear();

    seed_ids.clear();
}

inline void GreedyFinder::ProtoTrack::push_back(const hit_p_t &hit)
{
    candidate.push_back(hit);
    x.push_back(hit->GetX());
    y.push_back(hit->GetY());
    z.push_back(hit->GetZ());
}

inline void GreedyFinder::ProtoTrack::pop_back()
{
    candidate.pop_back();
    x.pop_back();
    y.pop_back();
    z.pop_back();
}

inline GreedyFinder::ProtoTrack& GreedyFinder::ProtoTrack::operator =(const seed_t &seed)
{
    this->clear();

    for(const auto &hit : seed())
    {
        this->push_back(hit);
        seed_ids.push_back(hit->GetCellIdZ());
    }

    return *this;
}

inline void GreedyFinder::ProtoTrackManager::reset(const GreedyFinderConfig &config, double *chi2, double *r2)
{
    best_chi2 = (chi2 == nullptr) ? config.kasa_chi2 : *chi2;
    best_r2   = (r2   == nullptr) ? config.linear_r2 : *r2;

    current_track.reset();
    current_track = std::make_shared<ProtoTrack>();
    current_chi2 = config.kasa_chi2;
    current_r2   = config.linear_r2;
    current_A = 0.;
    current_B = 0.;
    current_R = 0.;

    chosen_track.reset();
    chosen_chi2 = 0.;
    chosen_r2   = 0.;
    chosen_A = 0.;
    chosen_B = 0.;
    chosen_R = 0.;
}

inline void GreedyFinder::ProtoTrackManager::clear(const GreedyFinderConfig &config, double *chi2, double *r2)
{
    this->reset(config, chi2, r2);

    output_tracks.clear();
    output_chi2s.clear();
    output_r2s.clear();
    output_As.clear();
    output_Bs.clear();
    output_Rs.clear();
}

inline void GreedyFinder::ProtoTrackManager::Choose()
{
    chosen_track = std::make_shared<ProtoTrack>(*current_track);
    chosen_chi2 = current_chi2;
    chosen_r2 = current_r2;
    chosen_A = current_A;
    chosen_B = current_B;
    chosen_R = current_R;
}

template<class id_container_t>
inline void GreedyFinder::ProtoTrackManager::Remove(const id_container_t &removed_ids)
{
    size_t index = 0;
    auto rm_track = [&](std::vector<auto> &tracks)
                    {
                     tracks.erase(std::remove_if(tracks.begin(), tracks.end(),
                                               [&removed_ids, &index](const auto &)
                                               { return removed_ids.count(index++); }),
                                  tracks.end()
                                 );
                    };

    rm_track(output_tracks);
    rm_track(output_chi2s);
    rm_track(output_r2s);
    rm_track(output_As);
    rm_track(output_Bs);
    rm_track(output_Rs);
}

inline void GreedyFinder::ProtoTrackManager::Output()
{
    if(!chosen_track) return;

    output_tracks.emplace_back(std::move(chosen_track));
    output_chi2s.push_back(chosen_chi2);
    output_r2s.push_back(chosen_r2);
    output_As.push_back(chosen_A);
    output_Bs.push_back(chosen_B);
    output_Rs.push_back(chosen_R);
}
