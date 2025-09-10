file(GLOB_RECURSE Service_Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if (BUILD_ONNX)
    list(APPEND Service_Sources "${CMAKE_CURRENT_SOURCE_DIR}/../Onnx/src/onnx_runner.cpp")
endif ()

add_library(DService SHARED ${Service_Headers} ${Service_Sources})
if (BUILD_ONNX)
    target_link_libraries(DService PUBLIC onnxruntime)
endif ()

install(TARGETS DService LIBRARY DESTINATION lib )
