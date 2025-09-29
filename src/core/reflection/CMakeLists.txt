
target_sources(core-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Any.cppm
        ClassReflection.cppm
        EnTTComponentAttribute.cppm
        EnumReflection.cppm
        Reflect.cppm
        ReflectionContext.cppm
        TypeId.cppm
        TypeInfo.cppm
        TypeInfoImpl.cppm
        TypeReflection.cppm
        TypeTraits.cppm
)

target_sources(core-lib PRIVATE
        Any.cpp
        ClassReflection.cpp
        EnumReflection.cpp
        ReflectionContext.cpp
        TypeId.cpp
        TypeInfo.cpp
        TypeInfoImpl.cpp
        TypeReflection.cpp
        TypeTraits.cpp
)

add_subdirectory(type_traits)
