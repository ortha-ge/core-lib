module;

#include <optional>
#include <string_view>
#include <unordered_map>

export module Core.ReflectionContext;

import Core.Any;
import Core.ClassReflection;
import Core.EnumReflection;
import Core.TypeId;
import Core.TypeReflection;

export namespace Core {

	class ClassReflectionBuilderBase {
	public:

		ClassReflection getReflection();

	protected:
		explicit ClassReflectionBuilderBase(TypeId typeId, std::string_view className);

		void property(ClassProperty property);

	private:

		TypeId mTypeId;
		ClassReflection mClassReflection;

	};

	class EnumReflectionBuilderBase {
	public:

		EnumReflection getReflection();

	protected:

		explicit EnumReflectionBuilderBase(std::string_view name);

		void enumerator(std::string_view name, Any value);

	private:

		EnumReflection mReflection;

	};

	class ReflectionContext;

	template <class T>
	class ClassReflectionBuilder : public ClassReflectionBuilderBase {
	public:

		ClassReflectionBuilder(ReflectionContext& context, std::string_view className)
			: ClassReflectionBuilderBase(TypeId::get<T>(), className)
			, mContext(context) {
		}

		template <typename Member>
		ClassReflectionBuilder& property(std::string_view name, Member&& member) {
			const auto offset = memberOffset(member);
			const auto size = memberSize(member);
			const auto typeId = TypeId::get(member);
			ClassReflectionBuilderBase::property(ClassProperty(typeId, name, offset, size));
			return *this;
		}

		void build();

	private:

		ReflectionContext& mContext;

	};

	template <typename T>
	class EnumReflectionBuilder : public EnumReflectionBuilderBase {
	public:

		EnumReflectionBuilder(ReflectionContext& context, std::string_view name)
			: EnumReflectionBuilderBase(name)
			, mContext(context) {
		}

		EnumReflectionBuilder& constant(std::string_view name, T value) {
			Any valueAny(value);
			Any valueCopy = valueAny;
			enumerator(name, std::move(valueCopy));
			return *this;
		}

		void build();

	private:

		ReflectionContext& mContext;

	};

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

		template<class T>
		ClassReflectionBuilder<T> addClass(std::string_view name) {
			ClassReflectionBuilder<T> builder(*this, name);
			return builder;
		}

		template<class T>
		EnumReflectionBuilder<T> addEnum(std::string_view name) {
			EnumReflectionBuilder<T> builder(*this, name);
			return builder;
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

	template<class T>
	void ClassReflectionBuilder<T>::build() {
		mContext.addClass(TypeId::get<T>(), getReflection());
	}

	template<typename T>
	void EnumReflectionBuilder<T>::build() {
		mContext.addEnum(TypeId::get<T>(), getReflection());
	}

} // Core
