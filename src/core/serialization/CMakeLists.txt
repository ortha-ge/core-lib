
target_sources(ortha-core PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        JsonTypeLoaderAdapter.cppm
        JsonTypeSaverAdapter.cppm
        TypeLoader.cppm
        TypeSaver.cppm
)

target_sources(ortha-core PRIVATE
        JsonTypeLoaderAdapter.cpp
        JsonTypeSaverAdapter.cpp
)

add_subdirectory(services)
add_subdirectory(systems)
