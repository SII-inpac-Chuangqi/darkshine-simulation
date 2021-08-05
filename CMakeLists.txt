#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(DSimu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(OpenGL_GL_PREFERENCE GLVND)

# define path to macros and settings
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

INCLUDE(macros)
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
LINK_LIBRARIES(yaml-cpp)

find_package(XercesC REQUIRED)
if (XercesC_FOUND)
    INCLUDE_DIRECTORIES(${XercesC_INCLUDE_DIRS})
    LINK_LIBRARIES(${XercesC_LIBRARIES})
endif ()

option(DANA_TRACKING "DANA_TRACKING" ON)
if (DANA_TRACKING)
    # GenFit2
    option(USE_GENFIT "USE_GENFIT" ON)
    if (USE_GENFIT)

        FIND_PACKAGE(Eigen3 REQUIRED)
        INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
        MESSAGE("-- Eigen Dir: ${EIGEN3_INCLUDE_DIR}")
    else ()
        INCLUDE_DIRECTORIES(Utility/)
    endif ()
else ()
    add_definitions(-D_OFF_TRACKING=\"true\")
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

    if (${ROOT_VERSION} VERSION_GREATER_EQUAL "6.24.0")
        ADD_DEFINITIONS("-DRM_UNIT")
    endif ()
ENDIF ()


#----------------------------------------------------------------------------
# For Memory Check
#
option(MEMCK "MEMCK" OFF)
if (MEMCK OR ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    MESSAGE("-- Memory Check is ON")
    add_definitions("-DMEMCK")
endif ()

#---------------------------------------------------------------------------
# GenFit2
#
option(BUILD_GENFIT2 "BUILD_GENFIT2" ON)
if (BUILD_GENFIT2)
    add_subdirectory(GenFit2)
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

HEADER_DIRECTORIES(Algo_List Algo_File "Algorithms")
HEADER_DIRECTORIES(Util_List Util_File "Utility")
HEADER_DIRECTORIES_GENFIT2(GenFit2_List GenFit2_File "GenFit2")
MESSAGE(${GenFit2_List})

include_directories(DP_simu/include
        DP_ana/include
        DP_display/include
        Tools/include
        ./
        ${GenFit2_List}
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

file(GLOB Display_sources DP_display/src/*)
file(GLOB Display_headers DP_display/include/*)

file(GLOB Tools_sources Tools/src/*)
file(GLOB Tools_headers Tools/include/*)
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
        #Utility/Object/include/Object/DMagnet.h
        )
SET(AnaEvent_header_shared
        #DP_ana/include/Event/AnaEvent.h
        DP_ana/include/Event/EventReader.h
        DP_ana/include/Event/EventStoreAndWriter.h
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
        # Utility/Object/src/DMagnet.cpp
        )
SET(AnaEvent_source
        #DP_ana/src/AnaEvent.cpp
        DP_ana/src/EventReader.cpp
        DP_ana/src/EventStoreAndWriter.cpp
        )
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

# root_generate_dictionary(G__DAnaEvent ${AnaEvent_header_shared} LINKDEF Utility/UTIL/include/Utility/DAnaEventLinkDef.h)
#add_library(DAnaEvent SHARED ${AnaEvent_header_shared} ${AnaEvent_source} G__DAnaEvent.cxx)
#target_link_libraries(DAnaEvent PUBLIC DEvent)

root_generate_dictionary(G__Display ${Display_header_shared} LINKDEF Utility/UTIL/include/Utility/DisplayLinkDef.h)
#add_library(Display SHARED ${Display_header_shared} ${Display_source} G__Display.cxx)
#target_link_libraries(Display PUBLIC DAnaEvent)

add_library(DarkShine SHARED
        ${DEvent_header_shared} ${DEvent_source} G__DEvent.cxx
        ${AnaEvent_header_shared} ${AnaEvent_source} # G__DAnaEvent.cxx
        ${Display_header_shared} ${Display_source} G__Display.cxx)
target_link_libraries(DarkShine PUBLIC genfit2)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
link_libraries(DarkShine)
add_executable(DSimu DP_simu.cc ${Simu_sources} ${Simu_headers})
# ${Util_sources} ${Util_headers})

target_link_libraries(DSimu
        ${Geant4_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${PYTHIA6_LIBRARIES})

add_executable(DAna DP_ana.cpp ${Ana_sources} ${Ana_headers}
        Utility/UTIL/include/Utility/Config.h
        Utility/UTIL/src/Config.cpp
        ${Algo_sources} ${Algo_headers})


add_executable(DDis DP_display.cpp ${Display_sources} ${Display_headers})
# ${Util_sources} ${Util_headers} {Algo_sources} ${Algo_headers} ${Ana_sources} ${Ana_headers}
#if(DANA_TRACKING)
#    target_link_libraries(DDis Display genfit2)
#else()
#    target_link_libraries(DDis Display )
#endif()

add_executable(DPlot Tools/DPlot.cpp ${Tools_sources} ${Tools_headers})
target_link_libraries(DPlot yaml-cpp)

add_executable(DDump Tools/DDump.cpp ${Tools_sources} ${Tools_headers})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory
#
set(RUN_SCRIPTS
        DP_simu/scripts/default.yaml
        DP_simu/scripts/init_vis.mac
        DP_simu/scripts/vis.mac
        DP_simu/scripts/gui.mac
        )

foreach (_script ${RUN_SCRIPTS})
    configure_file(
            ${PROJECT_SOURCE_DIR}/${_script}
            ${PROJECT_BINARY_DIR}/
            COPYONLY
    )
endforeach ()



#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DSimu DESTINATION bin)
install(TARGETS DAna DESTINATION bin)
install(TARGETS DDis DESTINATION bin)
install(TARGETS DPlot DESTINATION bin)
install(TARGETS DDump DESTINATION bin)
install(TARGETS DEvent LIBRARY DESTINATION lib )
#install(TARGETS DAnaEvent LIBRARY DESTINATION lib )
#install(TARGETS Display LIBRARY DESTINATION lib )
install(TARGETS DarkShine LIBRARY DESTINATION lib)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent.rootmap DESTINATION lib)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDAnaEvent_rdict.pcm DESTINATION lib)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDAnaEvent.rootmap DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDisplay_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDisplay.rootmap DESTINATION lib)
