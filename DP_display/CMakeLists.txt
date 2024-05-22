include_directories(include
        ${PROJECT_SOURCE_DIR}/DP_ana/include
        )

file(GLOB Display_sources src/*)
file(GLOB Display_headers include/*)

file(GLOB Ana_sources ${PROJECT_SOURCE_DIR}/DP_ana/src/*.cpp)
file(GLOB Ana_headers ${PROJECT_SOURCE_DIR}/DP_ana/include/*)

file(GLOB_RECURSE Util_sources ${PROJECT_SOURCE_DIR}/Utility/*/src/*.cpp)
file(GLOB_RECURSE Util_headers ${PROJECT_SOURCE_DIR}/Utility/*/include/*)

FILE(GLOB_RECURSE Algo_sources ${PROJECT_SOURCE_DIR}/Algorithms/*/src/*.cpp)
FILE(GLOB_RECURSE Algo_headers ${PROJECT_SOURCE_DIR}/Algorithms/*/include/*)

IF (NOT BUILD_HDF5)
    # Get a list of all .cpp files in the excluded directory
    file(GLOB_RECURSE Excluded_sources ${PROJECT_SOURCE_DIR}/Algorithms/ECAL_ML_IO/src/*.cpp)
    file(GLOB_RECURSE Excluded_headers ${PROJECT_SOURCE_DIR}/Algorithms/ECAL_ML_IO/include/*)

    # Remove all .cpp files in the excluded directory from Algo_sources
    list(REMOVE_ITEM Algo_sources ${Excluded_sources})
    list(REMOVE_ITEM Algo_headers ${Excluded_headers})
ENDIF ()

#----------------------------------------------------------------------------
# Generate ROOT library for DEvent
#
# Header
SET(Display_header_shared
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DEventDisplay.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DEventReader_dis.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DSMagneticField.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/CaloHitsDisplay.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/DisData.h
        ${PROJECT_SOURCE_DIR}/DP_ana/include/Event/EventReader.h
        ${PROJECT_SOURCE_DIR}/DP_ana/include/Event/EventStoreAndWriter.h
        )

# Source
SET(Display_source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DEventDisplay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DEventDisplay_Processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DEventDisplay_DetInfo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DEventDisplay_DrawEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DEventReader_dis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CaloHitsDisplay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DisData.cpp
        ${PROJECT_SOURCE_DIR}/DP_ana/src/EventReader.cpp
        ${PROJECT_SOURCE_DIR}/DP_ana/src/EventStoreAndWriter.cpp
        )
root_generate_dictionary(G__Display ${Display_header_shared} LINKDEF ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/DisplayLinkDef.h)

add_library(DDisplay SHARED ${Display_header_shared} ${Display_source} G__Display.cxx
                ${Ana_sources} ${Ana_headers}
                ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
                ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp
                ${Algo_sources} ${Algo_headers}
        )

IF (BUILD_HDF5)
    target_link_libraries(DDisplay PUBLIC genfit2 ${HDF5_CXX_LIBRARIES} ${HDF5_LINK_LIBS})
ELSE()
    target_link_libraries(DDisplay PUBLIC genfit2)
ENDIF()

IF (BUILD_ACTS)
    get_library_names(${Acts_libraries} Acts_LIB_NAMES)
    target_link_libraries(DDisplay PUBLIC tbb ${Acts_LIB_NAMES} ${Boost_LIBRARIES})
ENDIF()

add_executable(DDis DP_display.cpp ${Display_sources} ${Display_headers})

target_link_libraries(DDis PUBLIC DDisplay)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DDis DESTINATION bin)
install(TARGETS DDisplay LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDisplay_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDisplay.rootmap DESTINATION lib)