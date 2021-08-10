#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(include)

#----------------------------------------------------------------------------
# Find required packages)
#
file(GLOB_RECURSE Simu_sources src/*)
file(GLOB_RECURSE Simu_headers include/*)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(DSimu DP_simu.cc ${Simu_sources} ${Simu_headers})

target_link_libraries(DSimu
        ${Geant4_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${PYTHIA6_LIBRARIES})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DSimu DESTINATION bin)