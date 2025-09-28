 
cmake_minimum_required(VERSION 3.31)

Include(FetchContent)

FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG c556831ddc6fe26d2fce01c14c97205a9dad46d5
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tracy)

find_package(bgfx CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(kangaru CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

set(core-lib-libraries
        EnTT::EnTT
        glfw
        glm::glm
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

target_sources(core-lib
        PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${glm_DIR}../../../include/glm
        FILES ${glm_DIR}/../../include/glm/glm.cppm
)

target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES BASE_DIRS src)
target_compile_features(core-lib PUBLIC cxx_std_23)
target_link_libraries(core-lib PUBLIC ${core-lib-libraries})
target_include_directories(core-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (EMSCRIPTEN_BUILD)
#target_compile_options(core-lib PUBLIC -pthread)
endif()

add_subdirectory(src)
add_subdirectory(tests)
