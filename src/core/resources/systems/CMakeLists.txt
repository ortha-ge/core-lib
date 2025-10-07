
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        FileLoadSystem.cppm
        ResourceLoadSystem.cppm
)

target_sources(ortha-core PRIVATE
        FileLoadSystem.cpp
        ResourceLoadSystem.cpp
)
