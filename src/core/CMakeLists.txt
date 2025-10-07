
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GlobalSpatial.cppm
        Scheduler.cppm
        Spatial.cppm
        Timer.cppm
)

target_sources(ortha-core PRIVATE
        Scheduler.cpp
        Timer.cpp
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
