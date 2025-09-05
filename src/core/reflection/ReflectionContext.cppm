module;

#include <optional>
#include <string_view>
#include <unordered_map>

export module Core.ReflectionContext;

import Core.ClassReflection;
import Core.EnumReflection;
import Core.TypeId;
import Core.TypeReflection;

export namespace Core {

    class ReflectionContext {
    public:

        ReflectionContext();

        void addBasicType(TypeId typeId, TypeReflection typeReflection);
        [[nodiscard]] bool hasBasicType(const TypeId& typeId) const;
        [[nodiscard]] const TypeReflection& getBasicType(TypeId typeId) const;

        void addClass(TypeId typeId, ClassReflection classReflection);
        [[nodiscard]] bool hasClass(const TypeId& typeId) const;
        [[nodiscard]] const ClassReflection& getClass(const TypeId& typeId) const;

		void addEnum(TypeId typeId, EnumReflection enumReflection);
		[[nodiscard]] bool hasEnum(const TypeId& typeId) const;
		[[nodiscard]] const EnumReflection& getEnum(const TypeId& typeId) const;

		std::optional<TypeId> getTypeIdByName(const std::string& name) const;

        template <class T>
        [[nodiscard]] bool hasClass() const {
            return hasClass(TypeId::get<T>());
        }

        template <class T>
        [[nodiscard]] const ClassReflection& getClass() const {
            if (const TypeId typeId{ TypeId::get<T>() }; hasClass(typeId)) {
                return getClass(typeId);
            }

            static ClassReflection null("Null");
            return null;
        }

        template <typename T>
        [[nodiscard]] bool hasBasicType() const {
            return hasBasicType(TypeId::get<T>());
        }

        template <typename T>
        [[nodiscard]] const TypeReflection& getBasicType() const {
            if (const TypeId& typeId{ TypeId::get<T>() }; hasBasicType(typeId)) {
                return getBasicType(typeId);
            }

            static TypeReflection null("Null", 0zu);
            return null;
        }

    private:

        void initializeBasicTypes();

		size_t mNextTypeId{};
		std::unordered_map<std::string, TypeId> mTypeNameLookup;
		std::unordered_map<TypeId, TypeReflection> mBasicTypeReflections;
		std::unordered_map<TypeId, ClassReflection> mClassReflections;
		std::unordered_map<TypeId, EnumReflection> mEnumReflections;
    };

    template <typename T>
    void reflect(ReflectionContext&) {
        static_assert(false, "Undefined reflect call for type");
    }

    ReflectionContext& getCurrentReflectionContext() {
        static ReflectionContext reflectionContext;
        return reflectionContext;
    }

} // Core
