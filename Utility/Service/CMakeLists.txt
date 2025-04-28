file(GLOB_RECURSE Service_Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(DService SHARED ${Service_Headers} ${Service_Sources})

install(TARGETS DService LIBRARY DESTINATION lib )