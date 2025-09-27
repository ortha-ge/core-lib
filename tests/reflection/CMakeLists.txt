
target_sources(core-lib-tests PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
)

target_sources(core-lib-tests PRIVATE
        AnyTests.cpp
        ClassReflectionTests.cpp
        TypeIdTests.cpp
)