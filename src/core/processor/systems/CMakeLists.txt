
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ProcessErrorLoggingSystem.cppm
        ProcessErrorRetrySystem.cppm
)

target_sources(ortha-core PRIVATE
        ProcessErrorLoggingSystem.cpp
        ProcessErrorRetrySystem.cpp
)
