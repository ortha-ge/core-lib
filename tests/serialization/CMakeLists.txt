
target_sources(ortha-core-tests PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        JsonTypeAdapterTestClasses.cppm
)

target_sources(ortha-core-tests PRIVATE
        JsonTypeLoaderAdapterTests.cpp
        JsonTypeSaverAdapterTests.cpp
)