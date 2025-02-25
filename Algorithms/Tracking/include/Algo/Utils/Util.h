#ifndef TRACKING_UTILS_H
#define TRACKING_UTILS_H

//................................................................................//
//C++
#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include <tuple>
#include <type_traits>

//................................................................................//
//ROOT
#include "TMath.h"

//................................................................................//
//Tracking

//Physics process map
const std::map<std::string, int> ProcessTag = {{"",                      0},
                                               {"eBrem",                 1},
                                               {"conv",                  2},
                                               {"photonNuclear",         3},
                                               {"neutronInelastic",      4},
                                               {"hadElastic",            5},
                                               {"electronNuclear",       6},
                                               {"Decay",                 7},
                                               {"nCapture",              8},
                                               {"pi+Inelastic",          9},
                                               {"protonInelastic",       10},
                                               {"pi-Inelastic",          11},
                                               {"hBertiniCaptureAtRest", 12},
                                               {"compt",                 13},
                                               {"annihil",               14},
                                               {"muMinusCaptureAtRest",  15},
                                               {"eIoni",                 16},
                                               {"dInelastic",            17},
                                               {"kaon0LInelastic",       18},
                                               {"He3Inelastic",          19},
                                               {"GammaToMuPair",         20},
                                               {"phot",                  21},
                                               {"kaon0SInelastic",       22},
                                               {"alphaInelastic",        23},
                                               {"kaon+Inelastic",        24},
                                               {"tInelastic",            25},
                                               {"lambdaInelastic",       26},
                                               {"kaon-Inelastic",        27},
                                               {"sigma-Inelastic",       28},
                                               {"DMProcessDMBrem",       29}};

//................................................................................//
//Functions
//................................................................................//

namespace tracking
{

//If particle pass through trackers
bool InTagTrack(double x, double y, double z);
bool InRecTrack(double x, double y, double z);

//
void LinearFit(double abr[], double x[], double y[], int n);
inline double PointToLineDistance(double A, double B, double C, double x, double y)
{
    return std::abs(A*x + B*y + C)/std::sqrt(A*A + B*B);
}

namespace utils
{
//................................................................................//
// Template to check if type T is iterable: is_iterable<T>::value
// https://stackoverflow.com/a/29634934
namespace detail
{
    using std::begin;
    using std::end;

    // To allow ADL with custom begin/end 
    template <typename T>
    auto is_iterable_impl(bool)
    -> decltype (
        begin(std::declval<T&>()) != end(std::declval<T&>()), // begin/end and operator !=
        ++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator ++
        *begin(std::declval<T&>()), // operator*
        std::true_type{}
    );
 
    template <typename T>
    std::false_type is_iterable_impl(...);
}

    template <typename T>
    using is_iterable = decltype(detail::is_iterable_impl<T>(0));

//................................................................................//
// Template to remove pointer/smart pointer from type T: remove_pointer_s_t<T>
// https://stackoverflow.com/a/24315093
    template <typename T>
    class remove_pointer_s {
        template <typename U=T>
        static auto test(int) -> std::remove_reference<decltype(*std::declval<U>())>;
        static auto test(...) -> std::remove_cv<T>;
    
    public:
        using type = typename decltype(test(0))::type;
    };
    
    template <typename T>
    using remove_pointer_s_t = typename remove_pointer_s<T>::type;

//................................................................................//
// Template to loop containers by range
// Michael Mahn on https://stackoverflow.com/questions/38493591/range-for-loop-with-multiple-containers
//#define ALT2

#ifndef ALT2
template<typename T, std::size_t i = 0, std::size_t j = std::tuple_size<T>::value>
struct tuple_compare {
    static bool
    one_equal(T const& lhs, T const& rhs) {
        if constexpr(i == j) return false;
        else {
            return (std::get<i>(lhs) == std::get<i>(rhs) ||
            tuple_compare<T, i + 1, j>::one_equal(lhs, rhs));
        }
    }
};
#endif

template<typename... Conts>
struct container_ref_tuple {
    static auto constexpr get_begin{[](auto&&... args){return std::make_tuple(begin(args)...);}};

    typename std::invoke_result<decltype(&std::forward_as_tuple<Conts...>), Conts&&...>::type m_refs;

    struct iterator {
        typename std::invoke_result<decltype(get_begin), Conts&&...>::type m_iterators;

        decltype(auto)
        operator++() {
            apply([](auto&... args) {((++args), ...);}, m_iterators);
            return (*this);
        }

        #ifndef ALT2
        //Alternative 1(safe)
        //will stop when it reaches the end of the shortest container
        auto
        operator!=(iterator const& rhs) const {
            return !tuple_compare<decltype(m_iterators)>::one_equal(m_iterators, rhs.m_iterators);
        }
        #else
        //Alternative 2 (probably faster, but unsafe):
        //use only, if first container is shortest
        auto
        operator!=(iterator const& rhs) const {
            return std::get<0>(m_iterators) != std::get<0>(rhs.m_iterators);
        }
        #endif

        auto
        operator*() const {
            return apply([](auto&... args){return std::forward_as_tuple(*args...);}, m_iterators);
        }
    };

    auto
    begin() const {
        return iterator{apply(get_begin, m_refs)};
    }

    #ifndef ALT2
    //Alternative 1(safe)
    //will stop when it reaches the end of the shortest container
        static auto constexpr get_end{[](auto&&... args){return std::make_tuple(end(args)...);}};
    auto
    end() const {
        return iterator{apply(get_end, m_refs)};
    }
    #else
    //Alternative 2 (probably faster, but unsafe):
    //use only, if first container is shortest
    auto
    end() const {
        iterator ret;
        std::get<0>(ret.m_iterators) = std::end(std::get<0>(m_refs));
        return ret;
    }
    #endif
};

template<typename... Conts>
auto
make_zip(Conts&&... conts) {
    return container_ref_tuple<Conts...>{std::forward_as_tuple(conts...)};
}

} // namespace utils
} // namespace tracking

#endif // UTILS_H

