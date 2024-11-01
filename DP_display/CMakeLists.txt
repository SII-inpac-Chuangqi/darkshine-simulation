include_directories(include
        )

file(GLOB Display_sources src/*)
file(GLOB Display_headers include/*)

file(GLOB DAna_sources ${PROJECT_SOURCE_DIR}/DP_ana/src/*.cpp)

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
root_generate_dictionary(G__Display ${Display_header_shared} LINKDEF include/DisplayLinkDef.h)

add_library(DDisplay SHARED ${Display_header_shared} ${Display_source} G__Display.cxx
                ${Ana_sources} ${Ana_headers}
                ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
                ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp)

target_link_libraries(DDisplay PUBLIC DAlgo)

IF (BUILD_ACTS)
    get_library_names(${Acts_libraries} Acts_LIB_NAMES)
    target_link_libraries(DDisplay PUBLIC tbb ${Acts_LIB_NAMES} Boost::program_options Boost::unit_test_framework Boost::container tbb)
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
