module;

#include <functional>
#include <string>
#include <utility>

module Core.ReflectionContext;

import Core.TypeId;

namespace Core {

    ReflectionContext::ReflectionContext() {
        initializeBasicTypes();
    }

    void ReflectionContext::addBasicType(TypeId typeId, TypeReflection typeReflection) {
        if (mBasicTypeReflections.contains(typeId)) {
            return;
        }

        mBasicTypeReflections.emplace(typeId, std::move(typeReflection));
    }

    void ReflectionContext::addClass(TypeId typeId, ClassReflection classReflection) {
        if (mClassReflections.contains(typeId)) {
            return;
        }

        mClassReflections.emplace(typeId, std::move(classReflection));
    }

    bool ReflectionContext::hasClass(const TypeId& typeId) const {
        return mClassReflections.contains(typeId);
    }

    void ReflectionContext::initializeBasicTypes() {
        addBasicType(TypeId::get<std::string>(), TypeReflection("std::string", sizeof(std::string)));
        addBasicType(TypeId::get<int32_t>(), TypeReflection("int32_t", sizeof(int32_t)));
        addBasicType(TypeId::get<double>(), TypeReflection("double", sizeof(double)));
        addBasicType(TypeId::get<float>(), TypeReflection("float", sizeof(float)));
        addBasicType(TypeId::get<bool>(), TypeReflection("bool", sizeof(bool)));
    }

    const ClassReflection& ReflectionContext::getClass(const TypeId& typeId) const {
        return mClassReflections.at(typeId);
    }

} // Core
