module;

#include <algorithm>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

module Core.ClassReflection;

namespace Core {

    ClassProperty::ClassProperty(TypeId typeId, std::string_view name, uintptr_t offset, uintptr_t size)
        : mTypeId(std::move(typeId))
        , mName(name)
        , mOffset(offset) {
    }

    void ClassProperty::setTypeId(TypeId typeId) {
        mTypeId = std::move(typeId);
    }

    const TypeId& ClassProperty::getTypeId() const {
        return mTypeId;
    }

    const std::string& ClassProperty::getName() const {
        return mName;
    }

    uintptr_t ClassProperty::offset() const {
        return mOffset;
    }

    ClassReflection::ClassReflection(std::string_view name)
        : mName(name) {
    }

    void ClassReflection::addProperty(ClassProperty property) {
        mProperties.emplace_back(std::move(property));
    }

    bool ClassReflection::hasProperty(std::string_view propertyName) const {
        return std::ranges::find_if(mProperties, [&](const auto& property) {
           return property.getName() == propertyName;
        }) != mProperties.end();
    }

    const ClassProperty& ClassReflection::getProperty(std::string_view propertyName) const {
        if (auto it = std::ranges::find_if(mProperties, [&](const auto& property) {
           return property.getName() == propertyName;
        }); it != mProperties.end()) {
            return *it;
        }

        static ClassProperty nullProperty(TypeId::get<void>(), "NULL", 0, 0);
        return nullProperty;
    }

    void ClassReflection::forEachProperty(std::function<void(const ClassProperty&)> visitor) const {
        for (auto&& property : mProperties) {
            visitor(property);
        }
    }

    void ClassReflection::forEachProperty(std::function<void(ClassProperty&)> visitor) {
        for (auto&& property : mProperties) {
            visitor(property);
        }
    }

	const std::string& ClassReflection::getName() const {
	    return mName;
    }

    const std::vector<ClassProperty>& ClassReflection::getProperties() const {
        return mProperties;
    }

    ClassReflectionBuilderBase::ClassReflectionBuilderBase(TypeId typeId, std::string_view className)
        : mTypeId{ std::move(typeId) }
        , mClassReflection{ className } {
    }

    ClassReflection ClassReflectionBuilderBase::build() {
        return mClassReflection;
    }

    void ClassReflectionBuilderBase::property(ClassProperty property) {
        mClassReflection.addProperty(std::move(property));
    }

} // Core
