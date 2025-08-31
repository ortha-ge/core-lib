module;

#include <string_view>

module Core.TypeReflection;

namespace Core {

    TypeReflection::TypeReflection(std::string_view typeName, size_t size)
            : mTypeName(typeName)
            , mSize(size) {
    }

    void TypeReflection::setTypeId(TypeId typeId) {
        mTypeId = std::move(typeId);
    }

    const TypeId& TypeReflection::getTypeId() const {
        return mTypeId;
    }

    size_t TypeReflection::getSize() const {
        return mSize;
    }

} // Core
