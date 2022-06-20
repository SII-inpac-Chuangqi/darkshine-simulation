#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(include)

#----------------------------------------------------------------------------
# Find required packages)
#
file(GLOB_RECURSE Simu_sources src/*)
file(GLOB_RECURSE Simu_headers include/*)

SET(UTIL_LIST
        # headers
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/TruthManager.h
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/PhysicsProcessDef.h
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/DetectorPVDef.h

        # sources
        ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/TruthManager.cpp
        )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(DSimu DP_simu.cc ${Simu_sources} ${Simu_headers} ${UTIL_LIST})

target_link_libraries(DSimu
        ${Geant4_LIBRARIES} ${PYTHIA6_LIBRARIES})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DSimu DESTINATION bin)