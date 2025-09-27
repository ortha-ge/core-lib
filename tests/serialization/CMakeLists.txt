
target_sources(core-lib-tests PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        JsonTypeAdapterTestClasses.cppm
)

target_sources(core-lib-tests PRIVATE
        JsonTypeLoaderAdapterTests.cpp
        JsonTypeSaverAdapterTests.cpp
)