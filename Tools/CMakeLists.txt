include_directories(include)

file(GLOB Tools_sources src/*)
file(GLOB Tools_headers include/*)

add_executable(DPlot DPlot.cpp ${Tools_sources} ${Tools_headers})
add_executable(DDump DDump.cpp ${Tools_sources} ${Tools_headers})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS DPlot DESTINATION bin)
install(TARGETS DDump DESTINATION bin)
