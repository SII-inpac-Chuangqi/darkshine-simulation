#ifndef SERVICE_ONNX_RUNNER_H
#define SERVICE_ONNX_RUNNER_H

#include <cstdint>
#include <string>
#include <iostream>
#include <tuple>

#include "onnxruntime_cxx_api.h"

#include "Onnx/onnx_data.h"

namespace OnnxHelper
{
template<class type_t>
static int64_t NElement(const type_t &shape)
{
    int64_t size = 1;
    for(const auto &i : shape) size *= std::abs(i);
    return size;
}
}

class OnnxRunner
{
public:
    OnnxRunner(const std::string &model_name) 
    {
        env_ = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "model-explorer");
        session_options_ = new Ort::SessionOptions();

        session_ = new Ort::Session(*env_, model_name.c_str(), *session_options_);

        allocator_ = new Ort::AllocatorWithDefaultOptions();
        memory_info_ = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
    }

    ~OnnxRunner()
    {
        for(auto s : input_names_char_) delete [] s;
        for(auto s : output_names_char_) delete [] s;

        delete session_; session_ = nullptr;
        delete allocator_; allocator_ = nullptr;
        delete session_options_; session_options_ = nullptr;
        delete env_; env_ = nullptr;
    }
    
    auto Session() { return session_; }

    auto BatchSize(int64_t batch_size) { batch_size_ = batch_size; return batch_size_; }
    auto NPoints(int64_t num_points) { num_points_ = num_points; return num_points_; }

    std::tuple<std::vector<std::string>, std::vector<std::vector<std::int64_t>>> InputNamesAndShapes();
    std::tuple<std::vector<std::string>, std::vector<std::vector<std::int64_t>>> OutputNamesAndShapes();

    std::vector<std::vector<float>> Inference(OnnxData &data);

private:
    Ort::Session *session_{nullptr};

    Ort::Env *env_{nullptr};

    Ort::SessionOptions *session_options_{nullptr};
    Ort::AllocatorWithDefaultOptions *allocator_{nullptr};
    Ort::MemoryInfo memory_info_{nullptr};

    int64_t batch_size_{1};
    int64_t num_points_{50};

    std::vector<const char*> input_names_char_;
    std::vector<const char*> output_names_char_;
    std::vector<std::vector<std::int64_t>> input_shapes_;
    std::vector<std::vector<std::int64_t>> output_shapes_;
};

#endif // SERVICE_ONNX_RUNNER_H
