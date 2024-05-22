#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

include_directories(
        include
)

file(GLOB Ana_sources src/*.cpp)
file(GLOB Ana_headers include/*)

file(GLOB_RECURSE Util_sources ${PROJECT_SOURCE_DIR}/Utility/*/src/*.cpp)
file(GLOB_RECURSE Util_headers ${PROJECT_SOURCE_DIR}/Utility/*/include/*)

FILE(GLOB_RECURSE Algo_sources ${PROJECT_SOURCE_DIR}/Algorithms/*/src/*.cpp)
FILE(GLOB_RECURSE Algo_headers ${PROJECT_SOURCE_DIR}/Algorithms/*/include/*)


IF (NOT BUILD_HDF5)
    # Get a list of all .cpp files in the excluded directory
    file(GLOB_RECURSE Excluded_sources ${PROJECT_SOURCE_DIR}/Algorithms/ECAL_ML_IO/src/*.cpp)
    file(GLOB_RECURSE Excluded_headers ${PROJECT_SOURCE_DIR}/Algorithms/ECAL_ML_IO/include/*)

    # Remove all .cpp files in the excluded directory from Algo_sources
    list(REMOVE_ITEM Algo_sources ${Excluded_sources})
    list(REMOVE_ITEM Algo_headers ${Excluded_headers})
ENDIF ()

#----------------------------------------------------------------------------
# Add the executable
#
add_executable(DAna DP_ana.cpp ${Ana_sources} ${Ana_headers}
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp
        ${Algo_sources} ${Algo_headers})

target_link_libraries(DAna genfit2)

IF (BUILD_HDF5)
    target_link_libraries(DAna ${HDF5_CXX_LIBRARIES} ${HDF5_LINK_LIBS})
ENDIF()

IF (BUILD_ACTS)
    get_library_names(${Acts_libraries} Acts_LIB_NAMES)
    target_link_libraries(DAna tbb ${Acts_LIB_NAMES} ${Boost_LIBRARIES})
ENDIF()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DAna DESTINATION bin)

#----------------------------------------------------------------------------
# Add the UNITTEST executable
#
#add_executable(GNNTest UNITTEST/GNN_TEST.cpp ${Ana_sources} ${Ana_headers}
#        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
#        ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp
#        ${Algo_sources}/ ${Algo_headers})
#
#target_link_libraries(GNNTest PUBLIC genfit2 ${HDF5_CXX_LIBRARIES} ${HDF5_LINK_LIBS})