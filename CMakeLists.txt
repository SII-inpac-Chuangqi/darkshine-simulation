#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(DSimu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if (WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else ()
    find_package(Geant4 REQUIRED)
endif ()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find required packages)
#
find_package(HepMC REQUIRED)

find_package(XercesC REQUIRED)
if (XercesC_FOUND)
    INCLUDE_DIRECTORIES( ${XercesC_INCLUDE_DIRS} )
    INCLUDE_DIRECTORIES( ${XercesC_LIBRARIES} )
    LINK_LIBRARIES( ${XercesC_LIBRARIES} )
endif ()

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
FIND_PACKAGE(ROOT COMPONENTS Geom)
IF (ROOT_FOUND)
    INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR})
    LINK_LIBRARIES(${ROOT_LIBRARIES})
    LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})
ENDIF ()
#----------------------------------------------------------------------------
# Find Pythia6 (optional package)
#
find_package(Pythia6 QUIET)
if (PYTHIA6_FOUND)
    message(STATUS "G4 Examples: Pythia6 found. --> DP_simu example with Pythia6 enabled.")
    add_definitions(-DG4LIB_USE_PYTHIA)
else ()
    set(PYTHIA6_LIBRARIES "")
endif ()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

# Recursively Find Algorithm headers
MACRO(HEADER_DIRECTORIES return_list return_file search_directory)
    FILE(GLOB_RECURSE new_list ${search_directory}/*/include/*.h)
    SET(dir_list "")
    SET(file_list "")
    FOREACH (file_path ${new_list})
        GET_FILENAME_COMPONENT(file_dir ${file_path} PATH)
        get_filename_component(dir_path ${file_dir} DIRECTORY)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH ()
    LIST(REMOVE_DUPLICATES dir_list)
    LIST(REMOVE_DUPLICATES new_list)
    SET(${return_list} ${dir_list})
    SET(${return_file} ${new_list})
ENDMACRO()
HEADER_DIRECTORIES(Algo_List Algo_File "Algorithms")
HEADER_DIRECTORIES(Util_List Util_File "Utility")

include_directories(DP_simu/include
        DP_ana/include
        Utility/Eigen
        ./
        ${Util_List}
        ${Algo_List}
        ${Geant4_INCLUDE_DIR}
        ${HEPMC_INCLUDE_DIR})

file(GLOB Simu_sources DP_simu/src/* DP_simu/src/Geometry/*)
file(GLOB Simu_headers DP_simu/include/*)

file(GLOB Ana_sources DP_ana/src/*.cpp)
file(GLOB Ana_headers DP_ana/include/*)

file(GLOB_RECURSE Util_sources Utility/*/src/*.cpp)
file(GLOB_RECURSE Util_headers Utility/*/include/*)

FILE(GLOB_RECURSE Algo_sources Algorithms/*/src/*.cpp)
FILE(GLOB_RECURSE Algo_headers Algorithms/*/include/*)

#----------------------------------------------------------------------------
# Generate ROOT library for DEvent
#
SET(dict_header_shared
        Utility/Object/include/Object/DHit.h
        Utility/Object/include/Object/DParticle.h
        Utility/Object/include/Object/DStep.h
        Utility/Object/include/Object/SimulatedHit.h
        Utility/Object/include/Object/CalorimeterHit.h
        Utility/Object/include/Object/McParticle.h
        Utility/Object/include/Object/ReconstructedParticle.h
        Utility/Object/include/Object/DEvent.h
        )
SET(dict_source
        Utility/Object/src/SimulatedHit.cpp
        Utility/Object/src/CalorimeterHit.cpp
        Utility/Object/src/McParticle.cpp
        Utility/Object/src/ReconstructedParticle.cpp
        Utility/Object/src/DEvent.cpp
        )
root_generate_dictionary(G__DEvent ${dict_header_shared} LINKDEF Utility/UTIL/include/Utility/DEventLinkDef.h)
add_library(DEvent SHARED ${dict_header_shared} ${dict_source} G__DEvent.cxx)
#target_include_directories(DEvent PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
#target_link_libraries(DEvent PUBLIC ROOT::Core)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(DSimu DP_simu.cc ${Simu_sources} ${Simu_headers}
        ${Util_sources} ${Util_headers})

target_link_libraries(DSimu ${Geant4_LIBRARIES}
        ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
        ${PYTHIA6_LIBRARIES}
        DEvent)

link_libraries(DEvent)
add_executable(DAna DP_ana.cpp ${Ana_sources} ${Ana_headers}
        ${Util_sources} ${Util_headers}
        ${Algo_sources} ${Algo_headers})
#target_link_libraries(DAna DEvent)
# if pythia is compiled with g77, link with -lg2c instead.

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DSimu DESTINATION bin)
install(TARGETS DAna DESTINATION bin)
install(TARGETS DEvent LIBRARY DESTINATION lib )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent.rootmap DESTINATION lib)