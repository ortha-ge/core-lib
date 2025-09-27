
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ProcessErrorLoggingSystemService.cppm
        ProcessErrorRetrySystemService.cppm
)

target_sources(core-lib PRIVATE
)
