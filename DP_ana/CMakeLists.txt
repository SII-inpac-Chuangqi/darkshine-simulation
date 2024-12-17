#----------------------------------------------------------------------------
# Locate sources and headers for this project
#


file(GLOB DAna_Sources src/*.cpp)

#----------------------------------------------------------------------------
# Add the executable
#
add_executable(DAna DP_ana.cpp ${DAna_Headers} ${DAna_Sources}
               ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
               ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp)

target_link_libraries(DAna DAlgo)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DAna DESTINATION bin)

#----------------------------------------------------------------------------
# Add the UNITTEST executable
#
#add_executable(GNNTest UNITTEST/GNN_TEST.cpp ${Ana_sources} ${Ana_headers}
#        ${PROJECT_SOURCE_DIR}/Utility/UTIL/include/Utility/Config.h
#        ${PROJECT_SOURCE_DIR}/Utility/UTIL/src/Config.cpp
#        ${Algo_sources}/ ${Algo_headers})
#
#target_link_libraries(GNNTest PUBLIC genfit2 ${HDF5_CXX_LIBRARIES} ${HDF5_LINK_LIBS})
