#ifndef TRACKING_ACTS_HELPER_H
#define TRACKING_ACTS_HELPER_H

namespace ActsHelper
{
template <class T>
inline T toActsReferenceFrameV4(const T& v_in) {
    T v_out{
        v_in.z(),
        -v_in.x(),
        -v_in.y(),
        v_in.w()
    };
    return v_out;
}

template <class T>
inline T toActsReferenceFrameV3(const T& v_in) {
    T v_out{
        v_in.z(),
        -v_in.x(),
        -v_in.y(),
    };
    return v_out;
}

template <class T>
inline T fromActsReferenceFrameV4(const T& v_in) {
    return {-v_in.y(), -v_in.z(), v_in.x(), v_in.w()};
}

template <class T>
inline T fromActsReferenceFrameV3(const T& v_in) {
    return {-v_in.y(), -v_in.z(), v_in.x()};
}

template <class T>
inline T fromActsReferenceFrameD3(const T& v_in) {
    return {-v_in.fY, -v_in.fZ, v_in.fX};
}

}

#endif // TRACKING_ACTS_HELPER_H
