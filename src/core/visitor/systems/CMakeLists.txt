
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        EntityVisitorSystem.cppm
)

target_sources(ortha-core PRIVATE
        EntityVisitorSystem.cpp
)
