cmake_minimum_required(VERSION 3.3)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
 
# Specify search path for CMake modules to be loaded by include() 
# and find_package()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

set(LIBFS_VERSION_MAJOR 0)
set(LIBFS_VERSION_MINOR 2)
set(LIBFS_VERSION_PATCH 3)
set(LIBFS_VERSION "${LIBFS_VERSION_MAJOR}.${LIBFS_VERSION_MINOR}.${LIBFS_VERSION_PATCH}")

#find_package(Git)
if(Git_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} status
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        ERROR_VARIABLE RESULT_STRING
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string(LENGTH "${RESULT_STRING}" LENGTH_RESULT_STRING)

    if(${LENGTH_RESULT_STRING} EQUAL 0)

        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        string(SUBSTRING ${GIT_COMMIT_HASH} 0 7 GIT_COMMIT_HASH)
        set(VERSION_HASH ${GIT_COMMIT_HASH} )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-list HEAD --count
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

    endif()
endif()

project(libfs VERSION ${LIBFS_VERSION} LANGUAGES C)
 
if (MSVC)
list(APPEND LIBFS_COMPILE_FLAGS /W4 /WX)
elseif(EMSCRIPTEN)
list(APPEND LIBFS_COMPILE_FLAGS -Wall -Wextra -Werror)
else()
list(APPEND LIBFS_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror)
endif()

include(ConfigureChecks)

option(LIBFS_STATIC "Build a static library" ON)
option(LIBFS_SHARED "Build a shared library" ON)
option(LIBFS_UNIT_TESTING "Unit Tests Enabled" ON)
option(LIBFS_DOXYGEN "Docs Enabled" OFF)

if (EMSCRIPTEN)
set(LIBFS_SHARED OFF)
set(LIBFS_UNIT_TESTING OFF)
set(LIBFS_DOXYGEN OFF)
endif()

# disallow in-source build
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

message("Building for target : ${CMAKE_SYSTEM_NAME}")
message("PROJECT_VERSION: ${LIBFS_VERSION}")
message("VERSION_HASH: ${VERSION_HASH}")
configure_file(fs.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/fs.h)

# libfs
set(LIBFS_LIB libfs)

file(GLOB HEADERS fs.h)
set(SOURCES fs.c)

if (LIBFS_SHARED)
    add_library("${LIBFS_LIB}" SHARED "${HEADERS}" "${SOURCES}")
    set_target_properties(${LIBFS_LIB} PROPERTIES
            C_STANDARD 90)

    target_compile_options("${LIBFS_LIB}"
                            PRIVATE
                                ${LIBFS_COMPILE_FLAGS}
                                -DHAVE_CONFIG_H)
                                
    target_include_directories(${LIBFS_LIB}
                                PRIVATE
                                    ${CMAKE_CURRENT_BINARY_DIR}
                                PUBLIC
                                    ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if (LIBFS_STATIC)
    
    set(LIBFS_STATIC_LIB "${LIBFS_LIB}-static")

    if (EMSCRIPTEN)
        add_executable(${LIBFS_STATIC_LIB} "${HEADERS}" "${SOURCES}")
    else()
        add_library(${LIBFS_STATIC_LIB} STATIC "${HEADERS}" "${SOURCES}")
    endif(EMSCRIPTEN)
    
    set_target_properties(${LIBFS_STATIC_LIB} PROPERTIES
if (not EMSCRIPTEN)
            C_STANDARD 90
endif()
            OUTPUT_NAME "${LIBFS_LIB}"
            PREFIX "")

    if (EMSCRIPTEN)

        set_target_properties(${LIBFS_STATIC_LIB}
            PROPERTIES LINK_FLAGS
                "-O2 -g0 -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=\"libfs\" -s FORCE_FILESYSTEM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=['_malloc','_free'] -s EXPORTED_RUNTIME_METHODS=['ccall','allocateUTF8','FS','NODEFS'] --post-js ${CMAKE_CURRENT_SOURCE_DIR}/dist/fs.js -lnodefs.js")
    
    endif (EMSCRIPTEN)

    target_compile_options(${LIBFS_STATIC_LIB}
                           PRIVATE
                               ${LIBFS_COMPILE_FLAGS}
                               -DHAVE_CONFIG_H
                               -DLIBFS_HIDE_SYMBOLS)
                               
    target_include_directories(${LIBFS_STATIC_LIB}
                               PRIVATE
                                   ${CMAKE_CURRENT_BINARY_DIR}
                               PUBLIC
                                    ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# include cmocka
if (LIBFS_UNIT_TESTING)
    if (NOT LIBFS_STATIC)
        message("Skip unit testing because LIBFS_STATIC option is off")

    else()
        include(AddCMockaTest) 
        add_subdirectory(tests)

    endif(NOT LIBFS_STATIC)
endif (LIBFS_UNIT_TESTING)

if (LIBFS_DOXYGEN)
    add_subdirectory ("docs")
endif (LIBFS_DOXYGEN)