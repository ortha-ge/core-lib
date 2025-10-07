
target_sources(ortha-core-tests PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
)

target_sources(ortha-core-tests PRIVATE
        AnyTests.cpp
        ClassReflectionTests.cpp
        TypeIdTests.cpp
)