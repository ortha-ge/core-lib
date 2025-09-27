
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Process.cppm
        ProcessError.cppm
        ProcessErrorLoggedFlag.cppm
)

target_sources(core-lib PRIVATE
        ProcessError.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
