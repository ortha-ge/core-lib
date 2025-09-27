
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        NodeParentGlobalSpatialUpdateSystem.cppm
)

target_sources(core-lib PRIVATE
        NodeParentGlobalSpatialUpdateSystem.cpp
)
