
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Any.cppm
        BasicTypeTraits.cppm
        ClassReflection.cppm
        EnTTComponentAttribute.cppm
        EnumReflection.cppm
        MapTypeTraits.cppm
        OptionalTypeTraits.cppm
        Reflect.cppm
        ReflectionContext.cppm
        TypeId.cppm
        TypeInfo.cppm
        TypeInfoImpl.cppm
        TypeReflection.cppm
        TypeTraits.cppm
        VariantTypeTraits.cppm
        VectorTypeTraits.cppm
)

target_sources(core-lib PRIVATE
        Any.cpp
        BasicTypeTraits.cpp
        ClassReflection.cpp
        EnumReflection.cpp
        MapTypeTraits.cpp
        OptionalTypeTraits.cpp
        ReflectionContext.cpp
        TypeId.cpp
        TypeInfo.cpp
        TypeInfoImpl.cpp
        TypeReflection.cpp
        TypeTraits.cpp
        VariantTypeTraits.cpp
        VectorTypeTraits.cpp
)
