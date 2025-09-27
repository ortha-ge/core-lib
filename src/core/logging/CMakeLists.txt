
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Log.cppm
        LogReportedFlag.cppm
)

target_sources(core-lib PRIVATE
        Log.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
