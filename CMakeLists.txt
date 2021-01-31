#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(DSimu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(OpenGL_GL_PREFERENCE GLVND)
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

find_package(GSL REQUIRED)
LINK_LIBRARIES(${GSL_LIBRARIES})

# yaml-cpp
find_package(yaml-cpp REQUIRED)
LINK_LIBRARIES( yaml-cpp )

find_package(XercesC REQUIRED)
if (XercesC_FOUND)
    INCLUDE_DIRECTORIES( ${XercesC_INCLUDE_DIRS} )
    LINK_LIBRARIES( ${XercesC_LIBRARIES} )
endif ()

# GenFit2
option(USE_GENFIT "USE_GENFIT" OFF)
if (USE_GENFIT)
    MESSAGE("-- USING GenFit2: ${USE_GENFIT}")
    INCLUDE_DIRECTORIES($ENV{GENFIT_INCLUDE_DIR})
    LINK_LIBRARIES( genfit2 )
    LINK_DIRECTORIES($ENV{GENFIT_LIB_DIR})

    FIND_PACKAGE(Eigen3 REQUIRED)
        INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
        MESSAGE("-- Eigen Dir: ${EIGEN3_INCLUDE_DIR}")
else()
    INCLUDE_DIRECTORIES(Utility/)
endif ()
#----------------------------------------------------------------------------
# Find ROOT (required package)
#
FIND_PACKAGE(OpenGL)

FIND_PACKAGE(ROOT COMPONENTS Geom TMVA Eve Gui RGL EG)
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
        DP_display/include
        Tools/include
        ./
        ${Util_List}
        ${Algo_List}
        ${Geant4_INCLUDE_DIR}
        ${HEPMC_INCLUDE_DIR})

file(GLOB_RECURSE Simu_sources DP_simu/src/*)

file(GLOB_RECURSE Simu_headers DP_simu/include/*)

file(GLOB Ana_sources DP_ana/src/*.cpp)
file(GLOB Ana_headers DP_ana/include/*)

file(GLOB_RECURSE Util_sources Utility/*/src/*.cpp)
file(GLOB_RECURSE Util_headers Utility/*/include/*)

FILE(GLOB_RECURSE Algo_sources Algorithms/*/src/*.cpp)
FILE(GLOB_RECURSE Algo_headers Algorithms/*/include/*)

file(GLOB Display_sources DP_display/src/* )
file(GLOB Display_headers DP_display/include/* )

file(GLOB Tools_sources Tools/src/* )
file(GLOB Tools_headers Tools/include/* )
#----------------------------------------------------------------------------
# Generate ROOT library for DEvent
#
# Header
SET(DEvent_header_shared
        Utility/Object/include/Object/DHit.h
        Utility/Object/include/Object/DParticle.h
        Utility/Object/include/Object/DStep.h
        Utility/Object/include/Object/SimulatedHit.h
        Utility/Object/include/Object/CalorimeterHit.h
        Utility/Object/include/Object/McParticle.h
        Utility/Object/include/Object/ReconstructedParticle.h
        Utility/Object/include/Object/DigiForm.hh
        Utility/Object/include/Object/DEvent.h
        )
SET(AnaEvent_header_shared
        DP_ana/include/Event/AnaEvent.h
        DP_ana/include/Event/EventStoreAndWriter.h
        DP_ana/include/Core/AnaProcessor.h

        # include All Running Ana Processors
        Algorithms/RecECAL/include/Algo/ECAL_Cluster.h
        Algorithms/RecECAL/include/Algo/Cluster_Analysis.h
        Algorithms/RecECAL/include/Algo/RecECAL.h
        )
SET(Display_header_shared
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/include/DEventDisplay.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/include/DEventReader_dis.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/include/DSMagneticField.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/include/CaloHitsDisplay.h
        )

# Source
SET(DEvent_source
        Utility/Object/src/SimulatedHit.cpp
        Utility/Object/src/CalorimeterHit.cpp
        Utility/Object/src/McParticle.cpp
        Utility/Object/src/ReconstructedParticle.cpp
        Utility/Object/src/DigiForm.cpp
        Utility/Object/src/DEvent.cpp
        )
SET(AnaEvent_source
        DP_ana/src/AnaEvent.cpp
        DP_ana/src/EventStoreAndWriter.cpp
        DP_ana/src/AnaProcessor.cpp

        # include All Running Ana Processors
        Algorithms/RecECAL/src/ECAL_Cluster.cpp
        Algorithms/RecECAL/src/Cluster_Analysis.cpp
        Algorithms/RecECAL/src/RecECAL.cpp
        DP_simu/include/DP_simu/StackingAction.hh)
SET(Display_source
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/src/DEventDisplay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/src/DEventDisplay_Processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/src/DEventDisplay_DetInfo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/src/DEventDisplay_DrawEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/src/DEventReader_dis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DP_display/src/CaloHitsDisplay.cpp
        )
root_generate_dictionary(G__DEvent ${DEvent_header_shared} LINKDEF Utility/UTIL/include/Utility/DEventLinkDef.h)
add_library(DEvent SHARED ${DEvent_header_shared} ${DEvent_source} G__DEvent.cxx)

root_generate_dictionary(G__DAnaEvent ${AnaEvent_header_shared} LINKDEF Utility/UTIL/include/Utility/DAnaEventLinkDef.h)
add_library(DAnaEvent SHARED ${AnaEvent_header_shared} ${AnaEvent_source} G__DAnaEvent.cxx)
target_link_libraries(DAnaEvent PUBLIC DEvent)

root_generate_dictionary(G__Display ${Display_header_shared} LINKDEF Utility/UTIL/include/Utility/DisplayLinkDef.h)
add_library(Display SHARED ${Display_header_shared} ${Display_source} G__Display.cxx)
target_link_libraries(Display PUBLIC DEvent DAnaEvent)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
link_libraries(DEvent)
add_executable(DSimu DP_simu.cc ${Simu_sources} ${Simu_headers}
        ${Util_sources} ${Util_headers} DP_simu/src/UserAction/StackingAction.cc)

target_link_libraries(DSimu ${Geant4_LIBRARIES}
        ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
        ${PYTHIA6_LIBRARIES})

add_executable(DAna DP_ana.cpp ${Ana_sources} ${Ana_headers}
        ${Util_sources} ${Util_headers}
        ${Algo_sources} ${Algo_headers})
target_link_libraries(DAna DAnaEvent)

add_executable(DDis DP_display.cpp ${Display_sources} ${Display_headers} ${Ana_sources} ${Ana_headers}
                                   ${Util_sources} ${Util_headers} ${Algo_sources} ${Algo_headers})
target_link_libraries(DDis Display)

add_executable(DPlot Tools/DPlot.cpp ${Tools_sources} ${Tools_headers})
target_link_libraries(DPlot yaml-cpp)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DSimu DESTINATION bin)
install(TARGETS DAna DESTINATION bin)
install(TARGETS DDis DESTINATION bin)
install(TARGETS DPlot DESTINATION bin)
install(TARGETS DEvent LIBRARY DESTINATION lib )
install(TARGETS DAnaEvent LIBRARY DESTINATION lib )
install(TARGETS Display LIBRARY DESTINATION lib )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDAnaEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDAnaEvent.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDisplay_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDisplay.rootmap DESTINATION lib)

