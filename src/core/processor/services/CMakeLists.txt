
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        ProcessErrorLoggingSystemService.cppm
        ProcessErrorRetrySystemService.cppm
)

target_sources(ortha-core PRIVATE
)
