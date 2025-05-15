#ifndef SEED_H
#define SEED_H

#include <memory>
#include <vector>
#include <functional>
#include <iostream>

#include "Algo/TypeDef.h"

template <typename hit_t>
class Seed
{
private:
    using hit_p_t = std::shared_ptr<hit_t>;

public:
    Seed() = default;
    Seed(const std::vector<hit_p_t> &bottoms, const hit_p_t &middle, const hit_p_t &top)
     : bottoms_(bottoms), middle_(middle), top_(top)
    {}
    Seed(const std::vector<hit_p_t> &bottoms, const hit_p_t &middle, const hit_p_t &top,
         const double &c1, const double &c2, const double &r0,
         const double &q1, const double &q2)
     : bottoms_(bottoms), middle_(middle), top_(top), c1_(c1), c2_(c2), r0_(r0),
       q1_(q1), q2_(q2)
    {}

    Seed(const Seed&) = default;
    Seed(Seed &&seed)
     : bottoms_(std::move(seed.bottoms_)), middle_(std::move(seed.middle_)), top_(std::move(seed.top_)),
       c1_(std::move(seed.c1_)), c2_(std::move(seed.c2_)), r0_(std::move(seed.r0_)),
       q1_(std::move(seed.q1_)), q2_(std::move(seed.q2_))
    {}

    Seed<hit_t>& operator=(const Seed<hit_t>&) = default;

    ~Seed() = default;

    void SetC1(const double &c1) {c1_ = c1;}
    void SetC2(const double &c2) {c2_ = c2;}
    void SetR0(const double &r0) {r0_ = r0;}

    double GetC1() const {return c1_;}
    double GetC2() const {return c2_;}
    double GetR0() const {return r0_;}
    double GetQ1() const {return q1_;}
    double GetQ2() const {return q2_;}

    auto GetBottoms() const {return bottoms_;}
    auto GetMiddle()  const {return middle_;}
    auto GetTop()     const {return top_;}

    auto GetCoords(std::function<double(const hit_t&)> get_coord) const
    {
        std::vector<double> coords;
        coords.reserve(1 + 1 + bottoms_.size());

        for(const auto &bottom : bottoms_) coords.push_back(get_coord(*bottom));
        coords.push_back(get_coord(*middle_));
        coords.push_back(get_coord(*top_));

        return coords;
    }

    std::vector<hit_p_t> operator() () const
    {
        auto hits = bottoms_;
        hits.push_back(middle_);
        hits.push_back(top_);

        return hits;
    }

    friend std::ostream& operator<<(std::ostream& os, const Seed &seed)
    {
        for(const auto &bottom : seed.bottoms_) os << *bottom << std::endl;
        os << *seed.middle_ << std::endl << *seed.top_ << std::endl;
        return os << "R: " << seed.r0_ << " \tq1: " << seed.q1_ << " \tq2: " << seed.q2_ << std::endl;
    }

private:
    std::vector<hit_p_t> bottoms_;
    hit_p_t middle_{nullptr};
    hit_p_t top_{nullptr};

    double c1_{0.};
    double c2_{0.};
    double r0_{0.};

    double q1_{0.};
    double q2_{0.};
};

#endif // SEED_H
