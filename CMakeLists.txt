#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(DSimu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(OpenGL_GL_PREFERENCE GLVND)

OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(MEMCK "MEMCK" OFF)
option(BUILD_ACTS "BUILD_ACTS" OFF)
option(BUILD_HDF5 "BUILD_HDF5" ON)

option(BUILD_DANA "BUILD_DANA" ON)
option(BUILD_DSIMU "BUILD_DSIMU" ON)
option(BUILD_DDIS "BUILD_DDIS" ON)
option(BUILD_TOOLS "BUILD_TOOLS" ON)

option(BUILD_ONNX "BUILD_ONNX" OFF)

# define path to macros and settings
#
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

INCLUDE(macros)

IF (ENABLE_GCOV AND NOT WIN32 AND NOT APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF()


#----------------------------------------------------------------------------
# Find required packages)
#
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
if (WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else ()
    find_package(Geant4 REQUIRED)
endif ()
include(${Geant4_USE_FILE})

# Find ROOT
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

# Find GSL
#
find_package(GSL REQUIRED)
LINK_LIBRARIES(${GSL_LIBRARIES})

# yaml-cpp
#
find_package(yaml-cpp REQUIRED)
LINK_LIBRARIES(yaml-cpp)

find_package(XercesC REQUIRED)
if (XercesC_FOUND)
    INCLUDE_DIRECTORIES(${XercesC_INCLUDE_DIRS})
    LINK_LIBRARIES(${XercesC_LIBRARIES})
endif ()

# GenFit2
#
IF (BUILD_DANA)
    FIND_PACKAGE(Eigen3 REQUIRED)
    INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIR})
    MESSAGE(STATUS "Eigen Dir: ${EIGEN3_INCLUDE_DIR}")
ENDIF ()

# ACTS
#
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
#
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
#
find_package(nlohmann_json 3.2.0 REQUIRED)

# Onnx
#
if (BUILD_ONNX)
    set(ONNXRUNTIME_INCLUDE_DIRS "/lustre/collider/zhuyifan/Software/onnxruntime/onnxruntime-linux-x64-1.11.1/include")
    include_directories(${ONNXRUNTIME_INCLUDE_DIRS})
endif ()

#----------------------------------------------------------------------------
# For Debug
#
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions("-DDSIMU_DEBUG")
endif ()


#----------------------------------------------------------------------------
# For Memory Check
#
if (MEMCK OR ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    MESSAGE("-- Memory Check is ON")
    add_definitions("-DMEMCK")
endif ()


#----------------------------------------------------------------------------
# Locate sources and headers for this project

FIND_DIRECTORIES(Object_Include_Dir Object_Headers "Utility/Object/include/*" "*.h")
FIND_DIRECTORIES(Util_Include_Dir   Util_Headers   "Utility/UTIL/include/*" "*.h")
FIND_DIRECTORIES(Service_Include_Dir   Service_Headers   "Utility/Service/include/*" "*.h")
FIND_DIRECTORIES(GenFit_Include_Dir GenFit_Headers "ThirdParty/GenFit2/*/include" "*.h" ON)
FIND_DIRECTORIES(Algo_Include_Dir   Algo_Headers   "Algorithms/*/include/Algo" "*.h")
FIND_DIRECTORIES(DAna_Include_Dir   DAna_Headers   "DP_ana/include/*" "*.h")

if (BUILD_ONNX)
    FIND_DIRECTORIES(Onnx_Include_Dir Onnx_Headers "Utility/Onnx/include/*" "*.h")
    list(APPEND Service_Headers ${Onnx_Headers})
endif ()

#message(STATUS "Object include dir: ${Object_Include_Dir}")
#message(STATUS "Object include headers: ${Object_Headers}")
#message(STATUS "Algo include dir: ${Algo_Include_Dir}")
#message(STATUS "GenFit2 include dir: ${GenFit_Include_Dir}")
include_directories(${Object_Include_Dir})
include_directories(${Util_Include_Dir})
include_directories(${Service_Include_Dir})
include_directories(${Algo_Include_Dir})
include_directories(${DAna_Include_Dir})
#include_directories(${HEPMC_INCLUDE_DIR})
include_directories(${GenFit_Include_Dir})
if (BUILD_ONNX)
    include_directories(${Onnx_Include_Dir})
endif ()

#----------------------------------------------------------------------------
# Core lib
#
# DEvent
#
add_subdirectory(Utility/Object)
link_libraries(DEvent)

# DService
add_subdirectory(Utility/Service)
link_libraries(DService)

#----------------------------------------------------------------------------
# Executables
#
# DSimu
#
if (BUILD_DSIMU)
    add_definitions(-DDSIMU_VERSION=\"1.7.0\")
    add_subdirectory(DP_simu)
endif ()

# DAna
#
if (BUILD_DANA)
    add_definitions(-DDANA_VERSION=\"1.7.0\")

    add_subdirectory(ThirdParty/GenFit2)
    add_subdirectory(Algorithms)
    add_subdirectory(DP_ana)
endif ()

# DDis
#
if (BUILD_DDIS)
    add_subdirectory(DP_display)
endif ()

# Tools
#
if (BUILD_TOOLS)
    add_subdirectory(Tools)
endif()


##----------------------------------------------------------------------------
## Copy all scripts to the build directory
##
#set(RUN_SCRIPTS
#        DP_simu/scripts/default.yaml
#        DP_simu/scripts/init_vis.mac
#        DP_simu/scripts/gui.mac
#        )
#
#foreach (_script ${RUN_SCRIPTS})
#    configure_file(
#            ${PROJECT_SOURCE_DIR}/${_script}
#            ${PROJECT_BINARY_DIR}/
#            COPYONLY
#    )
#endforeach ()

#----------------------------------------------------------------------------
# Generate setup.sh script
#
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/setup.sh.in
    ${CMAKE_BINARY_DIR}/setup.sh
    @ONLY
)

install(FILES ${CMAKE_BINARY_DIR}/setup.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX})
