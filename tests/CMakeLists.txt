
cmake_minimum_required(VERSION 3.31)

find_package(Catch2 3 REQUIRED)

set (core-lib-tests-src
        FileLoadSystemTests.cpp
        GLFWSystemTests.cpp
        reflection/ClassReflectionTests.cpp
        reflection/JsonTypeLoaderAdapterTests.cpp
        reflection/JsonTypeSaverAdapterTests.cpp
)

add_executable(core-lib-tests ${core-lib-tests-src})

target_link_libraries(core-lib-tests PRIVATE core-lib Catch2::Catch2WithMain)

add_custom_target(core-lib-tests-assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
    "$<TARGET_FILE_DIR:core-lib-tests>/assets/"
    COMMENT "Copying assets"
)

add_dependencies(core-lib-tests core-lib-tests-assets)
