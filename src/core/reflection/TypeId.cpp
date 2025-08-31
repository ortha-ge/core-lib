module;

#include <functional>

module Core.TypeId;

namespace Core {

    TypeIdInfo::TypeIdInfo(size_t size)
        : TypeIdInfo(TypeTraits{}, size) {
    }

    TypeIdInfo::TypeIdInfo(AllTypeTraits traits, size_t size)
        : mTypeTraits(traits)
        , mSize{ size } {
    }

    const AllTypeTraits& TypeIdInfo::getTypeTraits() const {
        return mTypeTraits;
    }

    size_t TypeIdInfo::getSize() const {
        return mSize;
    }

    TypeId::TypeId()
        : mTypeIdInfo(TypeId::get<void>().mTypeIdInfo) {
    }

    TypeId::TypeId(const TypeIdInfo& typeIdInfo)
        : mTypeIdInfo(&typeIdInfo) {
    }

    TypeId::TypeId(const TypeId&) = default;
    TypeId::TypeId(TypeId&&) noexcept = default;
    TypeId::~TypeId() = default;

    TypeId& TypeId::operator=(const TypeId&) = default;
    TypeId& TypeId::operator=(TypeId&&) noexcept = default;

    bool TypeId::operator==(const TypeId&) const = default;

    const TypeIdInfo& TypeId::getTypeIdInfo() const {
        if (mTypeIdInfo) {
            return *mTypeIdInfo;
        }

        static const TypeIdInfo invalidTypeInfo{ 0zu };
        return invalidTypeInfo;
    }

} // Core

size_t std::hash<Core::TypeId>::operator()(const Core::TypeId& typeId) const noexcept{
    return reinterpret_cast<size_t>(&typeId.getTypeIdInfo());
}
