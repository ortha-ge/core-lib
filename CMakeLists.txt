 
cmake_minimum_required(VERSION 3.31)

find_package(EnTT CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(kangaru CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

set(core-lib-src
        src/core/EnTTRegistry.cpp
        src/core/Scheduler.cpp
        src/core/Timer.cpp
        src/core/logging/Log.cpp
        src/core/logging/systems/LogReportingSystem.cpp
        src/core/platform/systems/GLFWSystem.cpp
        src/core/processor/systems/ProcessErrorLoggingSystem.cpp
        src/core/processor/systems/ProcessErrorRetrySystem.cpp
        src/core/reflection/Any.cpp
        src/core/reflection/BasicTypeTraits.cpp
        src/core/reflection/ClassReflection.cpp
        src/core/reflection/EnumReflection.cpp
        src/core/reflection/MapTypeTraits.cpp
        src/core/reflection/OptionalTypeTraits.cpp
        src/core/reflection/ReflectionContext.cpp
        src/core/reflection/TypeId.cpp
        src/core/reflection/TypeInfo.cpp
        src/core/reflection/TypeInfoImpl.cpp
        src/core/reflection/TypeReflection.cpp
        src/core/reflection/TypeTraits.cpp
        src/core/reflection/VectorTypeTraits.cpp
        src/core/resources/ResourceCache.cpp
        src/core/resources/ResourceHandle.cpp
        src/core/resources/ResourceLoadRequest.cpp
        src/core/resources/systems/FileLoadSystem.cpp
        src/core/resources/systems/ResourceLoadSystem.cpp
        src/core/serialization/JsonTypeLoaderAdapter.cpp
        src/core/serialization/JsonTypeSaverAdapter.cpp
        src/core/serialization/systems/TypeLoaderSystem.cpp
        src/core/systems/CoreSystems.cpp
)

set(core-lib-modules
        src/core/EnTTRegistry.cppm
        src/core/Scheduler.cppm
        src/core/Spatial.cppm
        src/core/Timer.cppm
        src/core/kangaru.cppm
        src/core/logging/Log.cppm
        src/core/logging/LogReportedFlag.cppm
        src/core/logging/systems/LogReportingSystem.cppm
        src/core/platform/GLFWWindow.cppm
        src/core/platform/NativeWindowHandles.cppm
        src/core/platform/Window.cppm
        src/core/platform/QuitAppRequest.cppm
        src/core/platform/systems/GLFWSystem.cppm
        src/core/processor/Process.cppm
        src/core/processor/ProcessError.cppm
        src/core/processor/systems/ProcessErrorLoggingSystem.cppm
        src/core/processor/systems/ProcessErrorRetrySystem.cppm
        src/core/reflection/Any.cppm
        src/core/reflection/BasicTypeTraits.cppm
        src/core/reflection/ClassReflection.cppm
        src/core/reflection/EnumReflection.cppm
        src/core/reflection/MapTypeTraits.cppm
        src/core/reflection/OptionalTypeTraits.cppm
        src/core/reflection/Reflect.cppm
        src/core/reflection/ReflectionContext.cppm
        src/core/reflection/TypeId.cppm
        src/core/reflection/TypeInfo.cppm
        src/core/reflection/TypeInfoImpl.cppm
        src/core/reflection/TypeReflection.cppm
        src/core/reflection/TypeTraits.cppm
        src/core/reflection/VectorTypeTraits.cppm
        src/core/resources/FileLoadRequest.cppm
        src/core/resources/RawDataResource.cppm
        src/core/resources/ResourceCache.cppm
        src/core/resources/ResourceHandle.cppm
        src/core/resources/ResourceLoadRequest.cppm
        src/core/resources/descriptors/FileDescriptor.cppm
        src/core/resources/systems/FileLoadSystem.cppm
        src/core/resources/systems/ResourceLoadSystem.cppm
        src/core/serialization/JsonTypeLoaderAdapter.cppm
        src/core/serialization/JsonTypeSaverAdapter.cppm
        src/core/serialization/TypeLoader.cppm
        src/core/serialization/TypeSaver.cppm
        src/core/serialization/systems/TypeLoaderSystem.cppm
        src/core/services/CoreSystemsService.cppm
        src/core/services/EnTTRegistryService.cppm
        src/core/services/SchedulerService.cppm
        src/core/services/TimerService.cppm
        src/core/systems/CoreSystems.cppm
)

set(core-lib-reflection-modules
)

add_library(core-lib STATIC ${core-lib-src} ${core-lib-modules} ${core-lib-reflection-modules})

target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ${core-lib-modules} ${core-lib-reflection-modules}
)
target_compile_features(core-lib PUBLIC cxx_std_23)
target_link_libraries(core-lib PUBLIC EnTT::EnTT glfw bx bimg bimg_decode)
target_include_directories(core-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests)
