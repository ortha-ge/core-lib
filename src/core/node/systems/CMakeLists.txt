
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        NodeParentGlobalSpatialUpdateSystem.cppm
)

target_sources(ortha-core PRIVATE
        NodeParentGlobalSpatialUpdateSystem.cpp
)
