
cmake_minimum_required(VERSION 3.31)

find_package(Catch2 3 REQUIRED)

set(core-lib-tests-src
        FileLoadSystemTests.cpp
        GLFWSystemTests.cpp
        reflection/AnyTests.cpp
        reflection/ClassReflectionTests.cpp
        reflection/TypeIdTests.cpp
        serialization/JsonTypeLoaderAdapterTests.cpp
        serialization/JsonTypeSaverAdapterTests.cpp
)

set(core-lib-tests-modules
        serialization/JsonTypeAdapterTestClasses.cppm
)

add_executable(core-lib-tests ${core-lib-tests-src})

target_sources(core-lib-tests PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${core-lib-tests-modules})
target_compile_features(core-lib-tests PUBLIC cxx_std_23)
target_link_libraries(core-lib-tests PRIVATE core-lib Catch2::Catch2WithMain)

add_custom_target(core-lib-tests-assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "$<TARGET_FILE_DIR:core-lib-tests>/assets/"
        COMMENT "Copying assets"
)

add_dependencies(core-lib-tests core-lib-tests-assets)

include(CTest)
include(Catch)
catch_discover_tests(core-lib-tests)
