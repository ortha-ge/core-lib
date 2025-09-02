 
cmake_minimum_required(VERSION 3.31)

find_package(EnTT CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(kangaru CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set (core-lib-src
        src/core/EnTTRegistry.cpp
        src/core/Scheduler.cpp
        src/core/reflection/Any.cpp
        src/core/reflection/ClassReflection.cpp
        src/core/reflection/ReflectionContext.cpp
        src/core/reflection/TypeId.cpp
        src/core/reflection/TypeReflection.cpp
        src/core/resources/ResourceCache.cpp
        src/core/resources/ResourceHandle.cpp
        src/core/resources/ResourceLoadRequest.cpp
        src/core/resources/systems/FileLoadSystem.cpp
        src/core/resources/systems/ImageLoadSystem.cpp
        src/core/resources/systems/ResourceLoadSystem.cpp
        src/core/serialization/JsonTypeLoaderAdapter.cpp
        src/core/serialization/JsonTypeSaverAdapter.cpp
        src/core/serialization/systems/TypeLoaderSystem.cpp
        src/core/systems/CoreSystems.cpp
        src/core/systems/GLFWSystem.cpp
)

set (core-lib-modules
        src/core/EnTTRegistry.cppm
        src/core/QuitAppRequest.cppm
        src/core/Scheduler.cppm
        src/core/Spatial.cppm
        src/core/Window.cppm
        src/core/WindowInternal.cppm
        src/core/kangaru.cppm
        src/core/reflection/Any.cppm
        src/core/reflection/ClassReflection.cppm
        src/core/reflection/ReflectionContext.cppm
        src/core/reflection/TypeId.cppm
        src/core/reflection/TypeReflection.cppm
        src/core/resources/FileLoadRequest.cppm
        src/core/resources/ImageResource.cppm
        src/core/resources/RawDataResource.cppm
        src/core/resources/ResourceCache.cppm
        src/core/resources/ResourceHandle.cppm
        src/core/resources/ResourceLoadRequest.cppm
        src/core/resources/SpriteResource.cppm
        src/core/resources/descriptors/FileDescriptor.cppm
        src/core/resources/descriptors/ImageDescriptor.cppm
        src/core/resources/systems/FileLoadSystem.cppm
        src/core/resources/systems/ImageLoadSystem.cppm
        src/core/resources/systems/ResourceLoadSystem.cppm
        src/core/serialization/JsonTypeLoaderAdapter.cppm
        src/core/serialization/JsonTypeSaverAdapter.cppm
        src/core/serialization/TypeLoader.cppm
        src/core/serialization/TypeSaver.cppm
        src/core/serialization/systems/TypeLoaderSystem.cppm
        src/core/services/CoreSystemsService.cppm
        src/core/services/EnTTRegistryService.cppm
        src/core/services/SchedulerService.cppm
        src/core/systems/CoreSystems.cppm
        src/core/systems/GLFWSystem.cppm
)

add_library(core-lib STATIC ${core-lib-src} ${core-lib-modules})

target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
    ${core-lib-modules}
)
target_compile_features(core-lib PUBLIC cxx_std_23)
target_link_libraries(core-lib PUBLIC EnTT::EnTT glfw bx bimg bimg_decode)
target_include_directories(core-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests)
