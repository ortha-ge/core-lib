
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Log.cppm
        LogReportedFlag.cppm
)

target_sources(ortha-core PRIVATE
        Log.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
