#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(DSimu)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find required packages)
#
find_package(HepMC REQUIRED)

#find_package(XercesC REQUIRED)
#if (XercesC_FOUND)
#    INCLUDE_DIRECTORIES( ${XercesC_INCLUDE_DIRS} )
#    INCLUDE_DIRECTORIES( ${XercesC_LIBRARIES} )
#    LINK_LIBRARIES( ${XercesC_LIBRARIES} )
#endif ()

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
FIND_PACKAGE( ROOT REQUIRED) 
IF( ROOT_FOUND )
    INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIRS} )
    INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIR} )
    LINK_LIBRARIES( ${ROOT_LIBRARIES} )
    LINK_DIRECTORIES( ${ROOT_LIBRARY_DIR} )
ENDIF()

#----------------------------------------------------------------------------
# Find Pythia6 (optional package)
#
find_package(Pythia6 QUIET)
if(PYTHIA6_FOUND)
  message(STATUS "G4 Examples: Pythia6 found. --> DP_simu example with Pythia6 enabled.") 
  add_definitions(-DG4LIB_USE_PYTHIA) 
else()
  set(PYTHIA6_LIBRARIES "")  
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

# Recursively Find Algorithm headers
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list Algorithms/*/include/*.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        get_filename_component(dir_path ${dir_path} DIRECTORY)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()
HEADER_DIRECTORIES(Algo_List)

include_directories(DP_simu/include
                    DP_ana/include
                    Utility/include
                    ${Algo_List}
                    ${Geant4_INCLUDE_DIR}
                    ${HEPMC_INCLUDE_DIR})

file(GLOB Simu_sources DP_simu/src/*.cc )
file(GLOB Simu_headers DP_simu/include/* )

file(GLOB Ana_sources DP_ana/src/*.cpp)
file(GLOB Ana_headers DP_ana/include/*)

file(GLOB Util_sources Utility/src/*.cpp)
file(GLOB Util_headers Utility/include/*)

FILE(GLOB_RECURSE Algo_sources Algorithms/*/src/*.cpp)
FILE(GLOB_RECURSE Algo_headers Algorithms/*/include/*.h)

MESSAGE("${Algo_sources}")
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(DSimu DP_simu.cc ${Simu_sources} ${Simu_headers} )
#target_link_libraries(DP_simu ${Geant4_LIBRARIES}
#                      ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
#                      ${PYTHIA6_LIBRARIES} gfortran)
target_link_libraries(DSimu ${Geant4_LIBRARIES}
                      ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
                      ${PYTHIA6_LIBRARIES})

add_executable(DAna DP_ana.cpp ${Ana_sources} ${Ana_headers}
                               ${Util_sources} ${Util_headers}
                               ${Algo_sources} ${Algo_headers})
#target_link_libraries(DAna  )
# if pythia is compiled with g77, link with -lg2c instead.
#target_link_libraries(DP_simu ${Geant4_LIBRARIES}
#                      ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
#                      ${PYTHIA6_LIBRARIES} g2c)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build DP_simu. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(DP_simu_SCRIPTS
        DP_simu/scripts/example.in
        DP_simu/scripts/gui.mac
        DP_simu/scripts/init.mac
        DP_simu/scripts/init_vis.mac
        DP_simu/scripts/vis.mac
  )


foreach(_script ${DP_simu_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DSimu DESTINATION bin)
install(TARGETS DAna DESTINATION bin)

