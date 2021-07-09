cmake_minimum_required(VERSION 3.3)
 
# Specify search path for CMake modules to be loaded by include() 
# and find_package()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

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

project(libfs VERSION ${PROJECT_VERSION} LANGUAGES C)

include(ConfigureChecks)
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

option(WITH_STATIC_LIB "Build a static library" ON)
option(UNIT_TESTING "Unit Tests Enabled" ON)

# disallow in-source build
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

message("Building for target : ${CMAKE_SYSTEM_NAME}")
message("PROJECT_VERSION: ${PROJECT_VERSION}")
message("VERSION_HASH: ${VERSION_HASH}")

configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/version.h")

# libfs
set(LIBFS_LIB libfs)

file(GLOB HEADERS fs.h)
set(SOURCES fs.c)

add_library("${LIBFS_LIB}" SHARED "${HEADERS}" "${SOURCES}")
target_compile_options("${LIBFS_LIB}"
                        PRIVATE
                            ${DEFAULT_C_COMPILE_FLAGS}
                            -DHAVE_CONFIG_H)
                               
target_include_directories(${LIBFS_LIB}
                            PRIVATE
                                ${CMAKE_CURRENT_BINARY_DIR}
                            PUBLIC
                                ${CMAKE_CURRENT_SOURCE_DIR})
if (WITH_STATIC_LIB)
    
    set(LIBFS_STATIC_LIB "${LIBFS_LIB}-static")

    add_library(${LIBFS_STATIC_LIB} STATIC "${HEADERS}" "${SOURCES}")
    set_target_properties(${LIBFS_STATIC_LIB} PROPERTIES OUTPUT_NAME "${LIBFS_LIB}")
    set_target_properties(${LIBFS_STATIC_LIB} PROPERTIES PREFIX "")

    target_compile_options(${LIBFS_STATIC_LIB}
                           PRIVATE
                               ${DEFAULT_C_COMPILE_FLAGS}
                               -DHAVE_CONFIG_H
                               -DLIBFS_HIDE_SYMBOLS)
                               
    target_include_directories(${LIBFS_STATIC_LIB}
                               PRIVATE
                                   ${CMAKE_CURRENT_BINARY_DIR}
                               PUBLIC
                                    ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# include cmocka
if (UNIT_TESTING)
 
    include(AddCMockaTest) 
    add_subdirectory(tests)
endif (UNIT_TESTING)