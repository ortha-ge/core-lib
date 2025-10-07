
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CoreSystemsService.cppm
        EnTTRegistryService.cppm
        GlobalSpatialUpdateSystemService.cppm
        SchedulerService.cppm
        TimerService.cppm
)

target_sources(ortha-core PRIVATE
        CoreSystemsService.cpp
)
