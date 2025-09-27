
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CoreSystems.cppm
        GlobalSpatialUpdateSystem.cppm
)

target_sources(core-lib PRIVATE
        CoreSystems.cpp
        GlobalSpatialUpdateSystem.cpp
)
