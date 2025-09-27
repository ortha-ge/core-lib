
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        EnTTRegistry.cppm
        GlobalSpatial.cppm
        Scheduler.cppm
        Spatial.cppm
        Timer.cppm
        kangaru.cppm
)

target_sources(core-lib PRIVATE
        EnTTRegistry.cpp
        Scheduler.cpp
        Timer.cpp
        kangaru.cpp
)

add_subdirectory(logging)
add_subdirectory(node)
add_subdirectory(platform)
add_subdirectory(processor)
add_subdirectory(reflection)
add_subdirectory(resources)
add_subdirectory(serialization)
add_subdirectory(services)
add_subdirectory(systems)
add_subdirectory(visitor)
