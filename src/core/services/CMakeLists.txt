
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CoreSystemsService.cppm
        EnTTRegistryService.cppm
        GlobalSpatialUpdateSystemService.cppm
        SchedulerService.cppm
        TimerService.cppm
)

target_sources(core-lib PRIVATE
        CoreSystemsService.cpp
)
