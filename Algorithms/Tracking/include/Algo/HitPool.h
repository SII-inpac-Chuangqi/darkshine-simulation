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

template <typename Key, typename Element>
class HitPool
{
private:
    using Pool = std::vector<std::shared_ptr<Element>>;
    using Map = std::map<Key, std::vector<std::shared_ptr<Element>>>;
    using KeyGetter = std::function<Key(const Element&)>;

public:
    HitPool()  {pool_.reserve(7); structured_ = new Map();}
    ~HitPool() {delete structured_; structured_ = nullptr;}
    HitPool(const Map&) = delete;

    Map* operator->()  {IsNull(); return structured_;}
    Map& operator*()   {IsNull(); return *structured_;}

    size_t Size() const {return pool_.size();}

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

    void AddHit(Element element)
    {
        pool_.emplace_back(std::make_shared<Element>(element));

        this->InsertMap();
    }

    std::shared_ptr<Element>& Back() {return pool_.back();}

    void Print()
    {
        if(IsNull()) return;

        for(auto &[key, layer] : *structured_)
        {
            std::cout << key << std::endl;

            for(auto &hit : layer)
            {
                //std::cout << hit << std::endl;
                std::cout << hit->GetX() << ",\t" << hit->GetY() << ",\t" << hit->GetZ() << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

private:
    Pool pool_;
    Map* structured_{nullptr};
    KeyGetter key_getter_;

    bool IsNull()
    {
        if(structured_) return false;

        std::cerr << "[WARNING] ==> Empty map" << std::endl;
        return true;
    }

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
