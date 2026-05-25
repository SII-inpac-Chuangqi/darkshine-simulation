# - Find ONNX Runtime C++ library
#
# This module finds the ONNX Runtime headers and shared library.
#
# Result variables:
#   ONNXRUNTIME_FOUND          - TRUE if found
#   ONNXRUNTIME_INCLUDE_DIRS   - Include directories
#   ONNXRUNTIME_LIBRARIES      - Library to link (full path)
#   ONNXRUNTIME_LIBRARY_DIR    - Directory containing the library
#
# Hints (CMake variables, not env):
#   ONNXRUNTIME_ROOT_DIR       - Top-level prefix
#   ONNX_ROOT_DIR              - Alias for ONNXRUNTIME_ROOT_DIR (legacy)

# Accept both ONNX_ROOT_DIR (legacy) and ONNXRUNTIME_ROOT_DIR
if(NOT ONNXRUNTIME_ROOT_DIR AND ONNX_ROOT_DIR)
    set(ONNXRUNTIME_ROOT_DIR ${ONNX_ROOT_DIR})
endif()

set(_search_paths)

if(ONNXRUNTIME_ROOT_DIR)
    list(APPEND _search_paths ${ONNXRUNTIME_ROOT_DIR})
endif()

# System paths
list(APPEND _search_paths /opt/onnxruntime /usr /usr/local)

# ---- CVMFS auto-detection ----
if(EXISTS "/cvmfs/sft.cern.ch/lcg/releases/onnxruntime")
    # Detect platform tuple
    execute_process(COMMAND uname -m OUTPUT_VARIABLE _arch OUTPUT_STRIP_TRAILING_WHITESPACE)

    set(_os_ver 7)
    if(EXISTS "/etc/centos-release")
        file(STRINGS /etc/centos-release _os_line)
        string(REGEX MATCH "release ([0-9]+)" _m ${_os_line})
        if(CMAKE_MATCH_1)
            set(_os_ver ${CMAKE_MATCH_1})
        endif()
    elseif(EXISTS "/etc/redhat-release")
        file(STRINGS /etc/redhat-release _os_line)
        string(REGEX MATCH "release ([0-9]+)" _m ${_os_line})
        if(CMAKE_MATCH_1)
            set(_os_ver ${CMAKE_MATCH_1})
        endif()
    endif()

    # Determine compiler prefix: gccN from CMAKE_CXX_COMPILER_VERSION
    set(_comp "gcc${CMAKE_CXX_COMPILER_VERSION}")
    string(REGEX REPLACE "^([0-9]+)\\.[0-9]+.*" "\\1" _gcc_major ${CMAKE_CXX_COMPILER_VERSION})

    # Glob all release dirs, pick newest matching this platform
    file(GLOB _rel_dirs "/cvmfs/sft.cern.ch/lcg/releases/onnxruntime/*")
    set(_best_ver "0.0.0")
    set(_best_dir "")

    foreach(_d ${_rel_dirs})
        if(NOT IS_DIRECTORY ${_d})
            continue()
        endif()
        # Look for platform build matching arch-centosVER-gccMAJOR
        file(GLOB _plat_dirs "${_d}/${_arch}-centos${_os_ver}-gcc${_gcc_major}*")
        list(LENGTH _plat_dirs _n)
        if(_n EQUAL 0)
            # Also try elN naming (el7, el8, el9)
            file(GLOB _plat_dirs "${_d}/${_arch}-el${_os_ver}-gcc${_gcc_major}*")
        endif()
        list(LENGTH _plat_dirs _n)
        if(_n GREATER 0)
            get_filename_component(_vname ${_d} NAME)
            string(REGEX REPLACE "^([0-9]+\\.[0-9]+\\.[0-9]+).*" "\\1" _ver ${_vname})
            if(${_ver} VERSION_GREATER ${_best_ver})
                set(_best_ver ${_ver})
                list(GET _plat_dirs 0 _best_dir)
            endif()
        endif()
    endforeach()

    if(_best_dir)
        list(APPEND _search_paths ${_best_dir})
        message(STATUS "ONNX Runtime CVMFS candidate: ${_best_dir}")
    endif()
endif()

# ---- Search ----
# Separate user/system paths from auto-detected CVMFS paths
# so CVMFS release takes priority over LCG view in PATH
set(_cvmfs_hints)
set(_user_pathes)
foreach(_p ${_search_paths})
    if(_p MATCHES "^/cvmfs/")
        list(APPEND _cvmfs_hints ${_p})
    else()
        list(APPEND _user_pathes ${_p})
    endif()
endforeach()

# First pass: prefer CVMFS (newer versions)
if(_cvmfs_hints)
    find_path(ONNXRUNTIME_INCLUDE_DIR
        NAMES onnxruntime_cxx_api.h
        HINTS ${_cvmfs_hints}
        PATH_SUFFIXES include include/core/session include/onnxruntime include/onnxruntime/core/session
        NO_DEFAULT_PATH
    )
    find_library(ONNXRUNTIME_LIBRARY
        NAMES onnxruntime
        HINTS ${_cvmfs_hints}
        PATH_SUFFIXES lib lib64
        NO_DEFAULT_PATH
    )
endif()

# Second pass: user paths + system defaults
if(NOT ONNXRUNTIME_INCLUDE_DIR OR NOT ONNXRUNTIME_LIBRARY)
    find_path(ONNXRUNTIME_INCLUDE_DIR
        NAMES onnxruntime_cxx_api.h
        HINTS ${_user_pathes}
        PATH_SUFFIXES include include/core/session include/onnxruntime include/onnxruntime/core/session
        DOC "ONNX Runtime include directory"
    )

    find_library(ONNXRUNTIME_LIBRARY
        NAMES onnxruntime
        HINTS ${_user_pathes}
        PATH_SUFFIXES lib lib64
        DOC "ONNX Runtime library"
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ONNXRUNTIME
    DEFAULT_MSG
    ONNXRUNTIME_INCLUDE_DIR
    ONNXRUNTIME_LIBRARY
)

if(ONNXRUNTIME_FOUND)
    set(ONNXRUNTIME_INCLUDE_DIRS ${ONNXRUNTIME_INCLUDE_DIR})
    set(ONNXRUNTIME_LIBRARIES ${ONNXRUNTIME_LIBRARY})
    get_filename_component(ONNXRUNTIME_LIBRARY_DIR ${ONNXRUNTIME_LIBRARY} DIRECTORY)
    message(STATUS "ONNX Runtime include: ${ONNXRUNTIME_INCLUDE_DIRS}")
    message(STATUS "ONNX Runtime library: ${ONNXRUNTIME_LIBRARIES}")
endif()

mark_as_advanced(ONNXRUNTIME_INCLUDE_DIR ONNXRUNTIME_LIBRARY)
