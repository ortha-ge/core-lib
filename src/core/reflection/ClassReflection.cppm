module;

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <string_view>

export module Core.ClassReflection;

import Core.TypeId;

export namespace Core {

    template <typename MemberType, typename T>
    uintptr_t memberOffset(MemberType T::* property) {
        const T* instance{ nullptr };
        return reinterpret_cast<uintptr_t>(&(instance->*property));
    }

    template <class T, typename MemberType>
    uintptr_t memberSize(MemberType T::* property) {
        return sizeof(MemberType);
    }

    class ClassProperty {
    public:

        ClassProperty(TypeId typeId, std::string_view name, uintptr_t offset, uintptr_t size);

        // Will be simplified as part of 'build'
        void setTypeId(TypeId typeId);
        const TypeId& getTypeId() const;

        template <typename MemberType>
        MemberType& apply(void* instance) const {
            return *reinterpret_cast<MemberType*>(static_cast<char*>(instance) + mOffset);
        }

        template <typename MemberType>
        const MemberType& apply(const void* instance) const {
            return *reinterpret_cast<const MemberType*>(static_cast<const char*>(instance) + mOffset);
        }

        template <typename MemberType, class T, typename = std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>, void*>>>
        const MemberType& apply(const T& instance) const {
            return *reinterpret_cast<const MemberType*>(reinterpret_cast<const char*>(&instance) + mOffset);
        }

        template <typename MemberType, class T, typename = std::enable_if_t<!std::is_same_v<std::remove_cvref_t<T>, void*>>>
        MemberType& apply(T& instance) const {
            return *reinterpret_cast<MemberType*>(reinterpret_cast<char*>(&instance) + mOffset);
        }

        void* getRawPointer(void* instance) const {
            return static_cast<char*>(instance) + mOffset;
        }

        const void* getRawPointer(const void* instance) const {
            return static_cast<const char*>(instance) + mOffset;
        }

        const std::string& getName() const;
        [[nodiscard]] uintptr_t offset() const;

    private:

        TypeId mTypeId;
        std::string mName;
        uintptr_t mOffset;

    };

    class ClassReflection {
    public:

        explicit ClassReflection(std::string_view name);
        ~ClassReflection() = default;
        ClassReflection(const ClassReflection&) = default;
        ClassReflection(ClassReflection&&) = default;

        ClassReflection& operator=(const ClassReflection&) = default;
        ClassReflection& operator=(ClassReflection&&) = default;

        void addProperty(ClassProperty property);
        bool hasProperty(std::string_view propertyName) const;
        const ClassProperty& getProperty(std::string_view propertyName) const;
        void forEachProperty(std::function<void(const ClassProperty&)> visitor) const;
        void forEachProperty(std::function<void(ClassProperty&)> visitor);

		const std::string& getName() const;
        const std::vector<ClassProperty>& getProperties() const;

    private:

        TypeId mTypeId{};
        std::string mName;
        std::vector<ClassProperty> mProperties;

    };

    class ClassReflectionBuilderBase {
    public:

        explicit ClassReflectionBuilderBase(TypeId typeId, std::string_view className);

        ClassReflection build();

    protected:

        void property(ClassProperty property);

    private:

        TypeId mTypeId;
        ClassReflection mClassReflection;

    };

    template <class T>
    class ClassReflectionBuilder : public ClassReflectionBuilderBase {
    public:

        explicit ClassReflectionBuilder(std::string_view className)
            : ClassReflectionBuilderBase(TypeId::get<T>(), className) {
        }

        template <typename Member>
        ClassReflectionBuilder& property(std::string_view name, Member&& member) {
            const auto offset = memberOffset(member);
            const auto size = memberSize(member);
            const auto typeId = TypeId::get(member);
            ClassReflectionBuilderBase::property(ClassProperty(typeId, name, offset, size));
            return *this;
        }

    };

} // Core
