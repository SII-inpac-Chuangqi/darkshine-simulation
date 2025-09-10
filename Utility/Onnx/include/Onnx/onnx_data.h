#ifndef SERVICE_ONNX_DATA_H
#define SERVICE_ONNX_DATA_H

#include <cassert>
#include <cstdint>
#include <vector>

struct OnnxData
{
    const int64_t batch_size;
    const int64_t N_points;

    std::vector<std::vector<int64_t>> pf_shapes;
    std::vector<std::vector<float>> pfs;

    OnnxData(const int64_t _batch_size,  const int64_t _N_points,
             const std::vector<std::vector<int64_t>> &_input_shapes)
             : batch_size(_batch_size), N_points(_N_points)
    {
        assert(_input_shapes.size() == 3);

        for(const auto &shape : _input_shapes)
        {
            pf_shapes.push_back({batch_size, shape.at(1), N_points});
            pfs.push_back({});
        }

    }

    void Clear()
    {
        for(auto &pf : pfs)
            pf.clear();
    }
};

#endif // SERVICE_ONNX_DATA_H
