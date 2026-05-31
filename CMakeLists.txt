cmake_minimum_required(VERSION 3.20)
project(
    AkaNetCore
    VERSION 0.5.1
    LANGUAGES CXX
)
set(AKANETCORE_VERSION_TAG "dev")
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/AkaNetCore/Version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Version.h
    @ONLY
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ANC_USE_BUNDLED_OPENSSL "Use bundled OpenSSL" ON)
option(ANC_USE_BUNDLED_PROTOBUF "Use bundled Protobuf" ON)
option(ANC_BUILD_GUI "Build GUI version" ON)
option(ANC_BUILD_CLI "Build CLI version" ON)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_library(AkaNetCore STATIC
    ${SOURCES}
)

target_include_directories(AkaNetCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

if(WIN32)
    target_compile_definitions(AkaNetCore
        PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
    )

    target_link_libraries(AkaNetCore
        ws2_32
        mswsock
    )
endif()

target_precompile_headers(AkaNetCore
    PRIVATE
        src/pch.h
)

# ----------------------------
# Tests
# ----------------------------

enable_testing()

add_subdirectory(tests)
