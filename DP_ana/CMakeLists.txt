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

#----------------------------------------------------------------------------
# Add the executable
#
add_executable(DAna DP_ana.cpp ${Ana_sources} ${Ana_headers}
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp
        ${Algo_sources} ${Algo_headers})

target_link_libraries(DAna genfit2)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DAna DESTINATION bin)
