#include "Onnx/onnx_runner.h"

#include <cassert>
#include <sstream>
#include <cstring>

std::string PrintShape(const std::vector<std::int64_t>& v)
{
    std::stringstream ss("");
    for (size_t i = 0; i < v.size() - 1; i++) ss << v[i] << "x";
    ss << v[v.size() - 1];
    return ss.str();
}

const char* StringToChar(const std::string& str) {
    char* char_str = new char[str.size() + 1];
    std::copy(str.c_str(), str.c_str() + str.size() + 1, char_str);
    return char_str;
}

std::tuple<std::vector<std::string>, std::vector<std::vector<std::int64_t>>> OnnxRunner::InputNamesAndShapes()
{
    std::vector<std::string> input_names;
    std::vector<std::vector<std::int64_t>> input_shapes;

    std::cout << "[INFO] ==> Input node name/shape:" << std::endl;
    for (size_t i = 0; i < session_->GetInputCount(); i++)
    {
        input_names.emplace_back(session_->GetInputName(i, *allocator_));
        input_shapes.emplace_back(session_->GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape());
        std::cout << "            " << input_names.at(i) << " : " << PrintShape(input_shapes.at(i)) << std::endl;
    }

    // some models might have negative shape values to indicate dynamic shape, e.g., for variable batch size.
    for (auto& s : input_shapes)
    {
        for (auto& i : s)
        { if (i < 0) i = 1; }
    }

    input_names_char_.resize(input_names.size(), nullptr);
    for (size_t i = 0; i < input_names.size(); i++)
        input_names_char_.at(i) = StringToChar(input_names.at(i));
    input_shapes_ = input_shapes;

    return {input_names, input_shapes};
}

std::tuple<std::vector<std::string>, std::vector<std::vector<std::int64_t>>> OnnxRunner::OutputNamesAndShapes()
{
    std::vector<std::string> output_names;
    std::vector<std::vector<std::int64_t>> output_shapes;

    std::cout << "[INFO] ==> Output node name/shape:" << std::endl;
    for (size_t i = 0; i < session_->GetOutputCount(); i++)
    {
        output_names.emplace_back(session_->GetOutputName(i, *allocator_));
        output_shapes.emplace_back(session_->GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape());
        std::cout << "            " << output_names.at(i) << " : " << PrintShape(output_shapes.at(i)) << std::endl;
    }

    output_names_char_.resize(output_names.size(), nullptr);
    for (size_t i = 0; i < output_names.size(); i++)
        output_names_char_.at(i) = StringToChar(output_names.at(i));
    output_shapes_ = output_shapes;

    return {output_names, output_shapes};
}

std::vector<std::vector<float>> OnnxRunner::Inference(OnnxData &data)
{
    auto &pf_shapes = data.pf_shapes;
    auto &pfs = data.pfs;

    assert(pf_shapes.size() == input_names_char_.size() && pf_shapes.size() == input_shapes_.size());
    assert(pfs.size() == input_names_char_.size() && pfs.size() == input_shapes_.size());

    // Create tensor objects
    std::vector<Ort::Value> input_tensors;
    for(size_t i = 0; i < pf_shapes.size(); i++)
        input_tensors.push_back(Ort::Value::CreateTensor<float>(memory_info_,
                                                                pfs.at(i).data(), pfs.at(i).size(),
                                                                pf_shapes.at(i).data(), pf_shapes.at(i).size()));

    // Create
    std::vector<std::vector<float>> output_datas;
    output_datas.reserve(output_names_char_.size());

    for(const auto &shape : output_shapes_)
    {
        auto N_element = OnnxHelper::NElement(shape);
        output_datas.emplace_back(batch_size_ * N_element);
    }

    // Run the model
    auto output_tensors = session_->Run(Ort::RunOptions{nullptr},
                                        input_names_char_.data(), input_tensors.data(), input_tensors.size(),
                                        output_names_char_.data(), output_names_char_.size());

    // Retrieve results
    for(size_t i = 0; i < output_names_char_.size(); i++)
    {
        const float* results = output_tensors[i].GetTensorData<float>();
        std::memcpy(output_datas.at(i).data(), results, output_datas.at(i).size() * sizeof(float));
    }

    return output_datas;
}
