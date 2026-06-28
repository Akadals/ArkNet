add_executable(AkaNetCoreTests
    ConsoleTest.cpp
 )

target_link_libraries(AkaNetCoreTests
    PRIVATE
        AkaNetCore
)

target_include_directories(AkaNetCoreTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(
    NAME AkaNetCoreTests
    COMMAND AkaNetCoreTests
)