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

MACRO(HEADER_DIRECTORIES_GENFIT2 return_list return_file search_directory)
    FILE(GLOB_RECURSE new_list ${search_directory}/*/include/*.h)
    SET(dir_list "")
    SET(file_list "")
    FOREACH (file_path ${new_list})
        get_filename_component(dir_path ${file_path} DIRECTORY)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH ()
    LIST(REMOVE_DUPLICATES dir_list)
    LIST(REMOVE_DUPLICATES new_list)
    SET(${return_list} ${dir_list})
    SET(${return_file} ${new_list})
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
