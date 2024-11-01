#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(DSimu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(OpenGL_GL_PREFERENCE GLVND)

# define path to macros and settings
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

INCLUDE(macros)

OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)
IF (ENABLE_GCOV AND NOT WIN32 AND NOT APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF()

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

# GenFit2
option(BUILD_DANA "BUILD_DANA" ON)
IF (BUILD_DANA)
    FIND_PACKAGE(Eigen3 REQUIRED)
    INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIR})
    MESSAGE(STATUS "Eigen Dir: ${EIGEN3_INCLUDE_DIR}")
ENDIF ()

# ACTS
option(BUILD_ACTS "BUILD_ACTS" OFF)
IF (BUILD_ACTS)
    FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options unit_test_framework container)
    FIND_PACKAGE(Acts REQUIRED COMPONENTS Core PluginJson)
ENDIF()
IF(Acts_FOUND)
#    find_package(TBB REQUIRED)

    ADD_DEFINITIONS(-DBUILD_ACTS)
    GET_TARGET_PROPERTY(Acts_include ActsCore INTERFACE_INCLUDE_DIRECTORIES)
    get_filename_component(Acts_DIR "${Acts_include}" DIRECTORY)
    INCLUDE_DIRECTORIES(${Acts_include})
    if (EXISTS "${Acts_DIR}/lib64")
        SET(Acts_libraries   "${Acts_DIR}/lib64")
    elseif (EXISTS "${Acts_DIR}/lib")
        SET(Acts_libraries   "${Acts_DIR}/lib")
    endif()
    LINK_DIRECTORIES(${Acts_libraries})
    MESSAGE(STATUS "Acts include dir: ${Acts_include}")
    MESSAGE(STATUS "Acts lib dir: ${Acts_libraries}")

    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    #LINK_DIRECTORIES(${Boost_LIBRARIES})
    MESSAGE(STATUS "Boost include dir: ${Boost_INCLUDE_DIR}")
    #MESSAGE(STATUS "Boost libs: ${Boost_LIBRARIES}")
ELSE()
    SET(BUILD_ACTS OFF)
ENDIF()

# HDF5
option(BUILD_HDF5 "BUILD_HDF5" ON)
IF (BUILD_HDF5)
    FIND_PACKAGE(HDF5 COMPONENTS C CXX)
    IF (HDF5_FOUND)
        ADD_DEFINITIONS(-DBUILD_HDF5)
        INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
        MESSAGE(STATUS "HDF5 Dir: ${HDF5_INCLUDE_DIRS}")
    ELSEIF ()
        SET(BUILD_HDF5 OFF)
    ENDIF ()
ENDIF ()
# json
find_package(nlohmann_json 3.2.0 REQUIRED)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
FIND_PACKAGE(OpenGL)

FIND_PACKAGE(ROOT COMPONENTS Geom TMVA Eve Gui RGL EG)
#FIND_PACKAGE(ROOT COMPONENTS Geom TMVA Gui EG)
IF (ROOT_FOUND)
    INCLUDE_DIRECTORIES(SYSTEM ${ROOT_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(SYSTEM ${ROOT_INCLUDE_DIR})
    LINK_LIBRARIES(${ROOT_LIBRARIES})
    LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})

    if (${ROOT_VERSION} VERSION_EQUAL "6.24.0")
        ADD_DEFINITIONS("-DRM_UNIT")
    endif ()
ENDIF ()

#----------------------------------------------------------------------------
# For Debug
#
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions("-DDSIMU_DEBUG")
endif ()

#----------------------------------------------------------------------------
# For Memory Check
#
option(MEMCK "MEMCK" OFF)
if (MEMCK OR ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    MESSAGE("-- Memory Check is ON")
    add_definitions("-DMEMCK")
endif ()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
HEADER_DIRECTORIES(Algo_List Algo_File "Algorithms")
HEADER_DIRECTORIES(Util_List Util_File "Utility")
HEADER_DIRECTORIES_GENFIT2(GenFit2_List GenFit2_File "ThirdParty/GenFit2")

include_directories(
        ./
        ${GenFit2_List}
        ${Util_List}
        ${Algo_List}
        ${Geant4_INCLUDE_DIR}
        ${HEPMC_INCLUDE_DIR}
)


#----------------------------------------------------------------------------
# DEvent
#
add_subdirectory(Utility/Object)
link_libraries(DEvent)

# DSimu
option(BUILD_DSIMU "BUILD_DSIMU" ON)
if (BUILD_DSIMU)
    add_definitions(-DDSIMU_VERSION=\"1.6.5\")
    add_subdirectory(DP_simu)
endif ()

#----------------------------------------------------------------------------
# DAna
#
option(BUILD_DDIS "BUILD_DDIS" ON)
if (BUILD_DANA)
    add_definitions(-DDANA_VERSION=\"v1.6.5\")

    add_subdirectory(ThirdParty/GenFit2)
    add_subdirectory(DP_ana)
    if (BUILD_DDIS)
        add_subdirectory(DP_display)
    endif ()
endif ()

#---------------------------------------------------------------------------
# Tools
#
option(BUILD_TOOLS "BUILD_TOOLS" ON)
if (BUILD_TOOLS)
    add_subdirectory(Tools)
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory
#
set(RUN_SCRIPTS
        DP_simu/scripts/default.yaml
        DP_simu/scripts/init_vis.mac
        DP_simu/scripts/gui.mac
        )

foreach (_script ${RUN_SCRIPTS})
    configure_file(
            ${PROJECT_SOURCE_DIR}/${_script}
            ${PROJECT_BINARY_DIR}/
            COPYONLY
    )
endforeach ()
