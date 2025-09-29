
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        BasicTypeTraits.cppm
        MapTypeTraits.cppm
        OptionalTypeTraits.cppm
        SharedPtrTypeTraits.cppm
        VariantTypeTraits.cppm
        VectorTypeTraits.cppm
)

target_sources(core-lib PRIVATE
        BasicTypeTraits.cpp
        MapTypeTraits.cpp
        OptionalTypeTraits.cpp
        SharedPtrTypeTraits.cpp
        VariantTypeTraits.cpp
        VectorTypeTraits.cpp
)
