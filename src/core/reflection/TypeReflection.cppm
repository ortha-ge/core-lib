module;

#include <string>
#include <string_view>

export module Core.TypeReflection;

import Core.TypeId;

export namespace Core {

    class TypeReflection {
    public:

        TypeReflection(std::string_view typeName, size_t size);
        ~TypeReflection() = default;
        TypeReflection(const TypeReflection&) = default;
        TypeReflection(TypeReflection&&) = default;

        TypeReflection& operator=(const TypeReflection&) = default;
        TypeReflection& operator=(TypeReflection&&) = default;

        void setTypeId(TypeId typeId);
        [[nodiscard]] const TypeId& getTypeId() const;

        [[nodiscard]] size_t getSize() const;

    private:

        TypeId mTypeId;
        std::string mTypeName;
        size_t mSize{};

    };

} // Core
