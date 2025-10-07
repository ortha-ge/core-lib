
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        CoreSystems.cppm
        GlobalSpatialUpdateSystem.cppm
)

target_sources(ortha-core PRIVATE
        CoreSystems.cpp
        GlobalSpatialUpdateSystem.cpp
)
