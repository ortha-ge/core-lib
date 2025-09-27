
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        LogReportingSystem.cppm
)

target_sources(core-lib PRIVATE
        LogReportingSystem.cpp
)
