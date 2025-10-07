
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GlobalSpatial.cppm
        Reflection.cppm
        Spatial.cppm
        glmVec2.cppm
        glmVec3.cppm
        glmVec4.cppm
)

target_sources(ortha-core PRIVATE
        Reflection.cpp
)

add_subdirectory(node)
