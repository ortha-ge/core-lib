
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        FileLoadSystem.cppm
        ResourceLoadSystem.cppm
)

target_sources(core-lib PRIVATE
        FileLoadSystem.cpp
        ResourceLoadSystem.cpp
)
