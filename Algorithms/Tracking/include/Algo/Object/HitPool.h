#ifndef TRACKING_HIT_POOL_H
#define TRACKING_HIT_POOL_H

//................................................................................//
//TRACKING
#include "Algo/TypeDef.h"

//................................................................................//
//C++
#include <memory>
#include <functional>
#include <vector>
#include <map>
#include <tuple>
#include <set>

template <typename Key, typename Element>
class HitPool
{
private:
    using Pool = std::vector<std::shared_ptr<Element>>;
    using Map = std::map<Key, std::vector<std::shared_ptr<Element>>>;
    using KeyGetter = std::function<Key(const Element&)>;
    using id_container_t = std::set<int>;

public:
    HitPool()  {this->Init();}
    ~HitPool() {this->Clear();}
    HitPool(const Pool&) = delete;

    Map* GetPool()     {IsNull(); return structured_;}
    Map* operator->()  {IsNull(); return structured_;}
    Map& operator*()   {IsNull(); return *structured_;}

    void Init() {pool_.reserve(7); structured_ = new Map();}

    void Clear()
    {
        delete structured_; structured_ = nullptr;
        pool_.clear();
    }

    bool IsNull()
    {
        if(structured_) return false;

        std::cerr << "[WARNING] ==> Empty map" << std::endl;
        return true;
    }

    size_t size() const {return pool_.size();}

    template <typename Arbitrary>
    void SetKeyGetter(std::function<Arbitrary(const Element&)> &f)
    {
        if(!IsSameKey(f))
        {
            std::cerr << "[WARNING] ==> Not return same type with key" << std::endl;
            return;
        }
        else
            key_getter_ = f;
    }

    void AddHit(std::shared_ptr<Element> &&element)
    {
        pool_.emplace_back(element);

        this->InsertMap();
    }

    void AddHit(const Element &element)
    {
        pool_.emplace_back(std::make_shared<Element>(element));

        this->InsertMap();
    }

    std::tuple<id_container_t, int, int> GetIds(const int &n_bottom_id, const int &shift = 0)
    {
        const int n_top_id = 1;
        const int n_middle_id = 1;

        if(std::abs(shift) + n_bottom_id + n_middle_id + n_top_id > static_cast<int>(structured_->size()))
        {
            std::cerr << "[WARNING] ==> Required total ids exceeds pool size" << std::endl;
            return std::make_tuple(id_container_t(), 0, 0);
        }

        id_container_t middel_ids;
        bottom_ids_.clear();

        auto it_top = std::next(structured_->rbegin(), shift > 0 ? 0 : -shift);
        top_id_ = it_top++->first;
        middle_id_ = it_top++->first;

        auto it_bottom = std::next(structured_->begin(), shift < 0 ? 0 : shift);
        bottom_ids_.insert(bottom_ids_.end(), it_bottom++->first);

        auto it_middle = 0;
        for(; it_middle < n_middle_id; it_middle++)
        {
            if     (it_middle%2 == 0 && it_bottom != structured_->end()) bottom_ids_.insert(bottom_ids_.end(), it_bottom++->first);
            else if(it_middle%2 == 1 && it_top != structured_->rend())   bottom_ids_.insert(bottom_ids_.end(), it_top++->first);
        }

        return std::make_tuple(bottom_ids_, middle_id_, top_id_);
    }

    std::shared_ptr<Element>& Back() {return pool_.back();}

    std::shared_ptr<Element>& Retrieve(Element *e_p)
    {
        for(const auto &e : pool_)
        {
            if(e.get() == e_p)
                return  e;
        }

        return nullptr;
    }

    void Print()
    {
        if(IsNull()) return;

        for(const auto &[key, layer] : *structured_)
        {
            std::cout << key << std::endl;

            for(const auto &hit : layer)
            {
                //std::cout << hit << std::endl;
                std::cout << hit->GetX() << ",\t" << hit->GetY() << ",\t" << hit->GetZ() << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

private:
    id_container_t bottom_ids_;
    size_t middle_id_{0};
    size_t top_id_{0};

    Pool pool_;
    Map *structured_{nullptr};
    KeyGetter key_getter_;

    template <typename Arbitrary>
    bool IsSameKey(std::function<Arbitrary(const Element&)> &f)
    {
        return std::is_same<std::decay_t<decltype(f(Element{}))>, Key>::value;
    }

    void InsertMap()
    {
        if(!key_getter_)
        {
            std::cerr << "[WARNING] ==> No key getter assigned" << std::endl;
            return;
        }

        if(IsNull()) return;

        Key key = key_getter_(*pool_.back());

        if(!structured_->insert(std::pair<Key, std::vector<std::shared_ptr<Element>>>(key, {pool_.back()})).second)
            structured_->at(key).push_back(pool_.back());
    }

};

#endif // TRACKING_HIT_POOL_H
