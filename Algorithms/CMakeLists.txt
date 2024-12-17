#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

file(GLOB_RECURSE Algo_Sources ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)
#message("-- Algo dirs: ${Algo_Include_Dir}")

add_library(DAlgo SHARED ${Algo_Headers} ${PROJECT_SOURCE_DIR}/DP_ana/include/Core/AnaProcessor.h ${PROJECT_SOURCE_DIR}/DP_ana/include/Core/AnaData.h
                         ${Algo_Sources} ${PROJECT_SOURCE_DIR}/DP_ana/src/AnaProcessor.cpp ${PROJECT_SOURCE_DIR}/DP_ana/src/AnaData.cpp
                         ${PROJECT_SOURCE_DIR}/Utility/Object/include/Object/Util/PhysicsProcessDef.h)
target_link_libraries(DAlgo genfit2)

IF (BUILD_HDF5)
    target_link_libraries(DAlgo ${HDF5_CXX_LIBRARIES} ${HDF5_LINK_LIBS})
ENDIF()

IF (BUILD_ACTS)
    get_library_names(${Acts_libraries} Acts_LIB_NAMES)
    target_link_libraries(DAlgo ${Acts_LIB_NAMES} Boost::program_options Boost::unit_test_framework Boost::container tbb)
ENDIF()

install(TARGETS DAlgo LIBRARY DESTINATION lib )
