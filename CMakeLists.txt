 
cmake_minimum_required(VERSION 3.31)

add_compile_definitions(
        GLM_GTC_INLINE_NAMESPACE
        GLM_GTX_INLINE_NAMESPACE
        GLM_ENABLE_EXPERIMENTAL
)

Include(FetchContent)

# Tracy
FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG c556831ddc6fe26d2fce01c14c97205a9dad46d5
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tracy)

# Glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/sweet-niamhy/glm.git
        GIT_TAG 	078805bfaaceaeeb3a6a06ccd9d77ca70eec53ee #cpp20_module_cmake
)

FetchContent_MakeAvailable(glm)

# entt
FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/sweet-niamhy/entt.git
        GIT_TAG ee64894b14c560839711804e1716925179591caa #cpp20_module
)

FetchContent_MakeAvailable(entt)

# kangaru
FetchContent_Declare(
        kangaru
        GIT_REPOSITORY  https://github.com/sweet-niamhy/kangaru.git
        GIT_TAG e57b8efad1b13ff5f70c57db4fa1a2e95e2713ab #cpp20_module
)

FetchContent_MakeAvailable(kangaru)

find_package(bgfx CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

set(core-lib-libraries
        ortha-rtti-lib
        EnTT::EnTT-Library
        glfw
        glm::glm
        kangaru::kangaru-library
        bgfx::bx
        bgfx::bimg
        bgfx::bimg_decode
        TracyClient
)

if (EMSCRIPTEN_BUILD)
find_package(SDL2 CONFIG REQUIRED)
set(core-lib-libraries ${core-lib-libraries} SDL2::SDL2)
endif()

add_library(core-lib STATIC)

target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES BASE_DIRS src)
target_compile_features(core-lib PUBLIC cxx_std_23)
target_link_libraries(core-lib PUBLIC ${core-lib-libraries})
target_include_directories(core-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (EMSCRIPTEN_BUILD)
#target_compile_options(core-lib PUBLIC -pthread)
endif()

add_subdirectory(src)
add_subdirectory(tests)
