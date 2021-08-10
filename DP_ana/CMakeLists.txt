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
# Generate ROOT library for DEvent
#
# Header
SET(AnaEvent_header_shared
        #${CMAKE_CURRENT_SOURCE_DIR}/include/Event/AnaEvent.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Event/EventReader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Event/EventStoreAndWriter.h
        )

#Source
SET(AnaEvent_source
        #${CMAKE_CURRENT_SOURCE_DIR}/src/AnaEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/EventReader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/EventStoreAndWriter.cpp
        )

add_library(DAnaEvent SHARED ${AnaEvent_header_shared} ${AnaEvent_source})

#----------------------------------------------------------------------------
# Add the executable
#
add_executable(DAna ${PROJECT_SOURCE_DIR}/DP_ana.cpp ${Ana_sources} ${Ana_headers}
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp
        ${Algo_sources} ${Algo_headers})

target_link_libraries(DAna genfit2)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DAna DESTINATION bin)
install(TARGETS DAnaEvent LIBRARY DESTINATION lib)