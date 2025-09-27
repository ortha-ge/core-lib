
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        JsonTypeLoaderAdapter.cppm
        JsonTypeSaverAdapter.cppm
        TypeLoader.cppm
        TypeSaver.cppm
)

target_sources(core-lib PRIVATE
        JsonTypeLoaderAdapter.cpp
        JsonTypeSaverAdapter.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
