project(libfs-tests C)

include(ExternalProject)

# CMocka
set(CMOCKA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/cmocka")
set(CMOCKA_INCLUDE_DIR "${CMOCKA_SOURCE_DIR}/include")
set(CMOCKA_BINARY_DIR "${CMOCKA_SOURCE_DIR}/build")

ExternalProject_Add(cmocka
  PREFIX cmocka
  SOURCE_DIR ${CMOCKA_SOURCE_DIR}
  BINARY_DIR ${CMOCKA_BINARY_DIR}
  INSTALL_COMMAND ""
  CMAKE_ARGS
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DHAVE_SNPRINTF=${HAVE_SNPRINTF}
        -DHAVE_VSNPRINTF=${HAVE_VSNPRINTF}
        -DWITH_STATIC_LIB=true
)
ExternalProject_Get_Property(cmocka source_dir binary_dir)

add_library(cmocka_vendor STATIC SHARED IMPORTED)

if(MSVC)
  set(CMOCKA_LIBRARY "${CMOCKA_BINARY_DIR}/src/Release/cmocka-static.lib")
else()
  set(CMOCKA_LIBRARY "${CMOCKA_BINARY_DIR}/src/libcmocka-static.a")
endif()

add_dependencies(cmocka_vendor cmocka)

# libfs tests
set(_TEST "test_fs")

message("cmocka_BINARY_DIR=${cmocka_BINARY_DIR}")
message("CMOCKA_STATIC_LIBRARY=${CMOCKA_STATIC_LIBRARY}")

include_directories(${CMOCKA_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR})

add_cmocka_test(libfs-${_TEST}
                SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${_TEST}.c
                COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
                LINK_LIBRARIES ${CMOCKA_LIBRARY} ${LIBFS_STATIC_LIB}
)
    
target_include_directories(libfs-${_TEST}
    PRIVATE 
        ${CMAKE_BINARY_DIR}
        ${cmocka_BINARY_DIR})

# Required for static linking CMocka
if (WIN32)
target_compile_options(libfs-${_TEST} PRIVATE "/MD$<$<CONFIG:Release>:>" "/MDd$<$<CONFIG:Debug>:>")
endif()

# Copy test files
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILE_PERMISSIONS OWNER_READ OWNER_WRITE)
