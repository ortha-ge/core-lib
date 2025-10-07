
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        LogReportingSystem.cppm
)

target_sources(ortha-core PRIVATE
        LogReportingSystem.cpp
)
