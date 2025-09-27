
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        TypeLoaderSystem.cppm
)

target_sources(core-lib PRIVATE
        TypeLoaderSystem.cpp
)
