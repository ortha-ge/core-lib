
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        GlobalSpatial.cppm
        Reflection.cppm
        Spatial.cppm
        glmVec2.cppm
        glmVec3.cppm
        glmVec4.cppm
)

target_sources(core-lib PRIVATE
        Reflection.cpp
)
