# Recursively Find Algorithm files
MACRO(FIND_DIRECTORIES return_list return_file search_directory file_surfix)
    set (extra_args ${ARGN})
    list(LENGTH extra_args extra_count)

    set (GenFit_flag "OFF")
    if(extra_count GREATER 0)
        list(GET extra_args 0 GenFit_flag)
    endif()

    FILE(GLOB_RECURSE file_list "${search_directory}/${file_surfix}")
    SET(dir_list "")
    FOREACH(file_path ${file_list})
        if(GenFit_flag STREQUAL "ON" OR GenFit_flag STREQUAL "on")
            GET_FILENAME_COMPONENT(dir_path ${file_path} DIRECTORY)
        else()
            GET_FILENAME_COMPONENT(file_dir ${file_path} PATH)
            GET_FILENAME_COMPONENT(dir_path ${file_dir} DIRECTORY)
        endif()
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${dir_list})
    SET(${return_file} ${file_list})
ENDMACRO()

# This macro takes one argument: MY_DIRECTORY, the directory to search for .so files.
macro(get_library_names MY_DIRECTORY RETURN_LIST)
    # Use the GLOB command to find all .so files in the specified directory.
    file(GLOB LIBRARY_FILES "${MY_DIRECTORY}/lib*.so")

    # Create an empty list to store the library names.
    set(LIBRARY_NAMES)

    # Iterate over each file path in LIBRARY_FILES.
    foreach(FILE_PATH ${LIBRARY_FILES})
        # Extract the file name without the directory path and extension.
        get_filename_component(LIB_NAME_WE ${FILE_PATH} NAME_WE)

        # Remove the 'lib' prefix from the library name.
        string(SUBSTRING ${LIB_NAME_WE} 3 -1 LIB_NAME)

        # Append the library name to the LIBRARY_NAMES list.
        list(APPEND LIBRARY_NAMES ${LIB_NAME})
    endforeach()

    # Return the list of library names to the caller.
    set(${RETURN_LIST} ${LIBRARY_NAMES})

endmacro()
