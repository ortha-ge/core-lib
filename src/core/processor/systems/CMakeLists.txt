
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ProcessErrorLoggingSystem.cppm
        ProcessErrorRetrySystem.cppm
)

target_sources(core-lib PRIVATE
        ProcessErrorLoggingSystem.cpp
        ProcessErrorRetrySystem.cpp
)
