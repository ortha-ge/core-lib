
cmake_minimum_required(VERSION 3.31)

find_package(Catch2 3 REQUIRED)

add_executable(ortha-core-tests)

target_sources(ortha-core-tests PUBLIC FILE_SET modules TYPE CXX_MODULES BASE_DIRS .)
target_sources(ortha-core-tests PRIVATE
    FileLoadSystemTests.cpp
        GLFWSystemTests.cpp
)

target_compile_features(ortha-core-tests PUBLIC cxx_std_23)
target_link_libraries(ortha-core-tests PRIVATE ortha-core Catch2::Catch2WithMain)

add_custom_target(ortha-core-tests-assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "$<TARGET_FILE_DIR:ortha-core-tests>/assets/"
        COMMENT "Copying assets"
)

add_dependencies(ortha-core-tests ortha-core-tests-assets)

add_subdirectory(reflection)
add_subdirectory(serialization)

include(CTest)
include(Catch)
catch_discover_tests(ortha-core-tests)
