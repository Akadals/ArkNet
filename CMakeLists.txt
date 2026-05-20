cmake_minimum_required(VERSION 3.20)

project(AkaNetCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_library(AkaNetCore STATIC
    ${SOURCES}
)

target_include_directories(AkaNetCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
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