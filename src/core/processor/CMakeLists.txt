
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Process.cppm
        ProcessError.cppm
        ProcessErrorLoggedFlag.cppm
)

target_sources(ortha-core PRIVATE
        ProcessError.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
