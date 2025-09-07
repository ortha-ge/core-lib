module;

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

export module Core.ReflectionContext;

import Core.Any;
import Core.ClassReflection;
import Core.EnumReflection;
import Core.Log;
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

	template<typename T>
	void reflect(ReflectionContext&) {
		static_assert(false, "Undefined reflect call for type");
	}

	template<typename T>
	class ClassReflectionBuilder;

	template<typename T>
	class EnumReflectionBuilder;

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
		[[nodiscard]] bool hasEnum() const {
			return hasEnum(TypeId::get<T>());
		}

		template<class T>
		[[nodiscard]] bool hasClass() const {
			return hasClass(TypeId::get<T>());
		}

		template<class T>
		[[nodiscard]] const ClassReflection& getClass() const {
			if (const TypeId typeId{ TypeId::get<T>() }; hasClass(typeId)) {
				return getClass(typeId);
			}

			static ClassReflection null("Null");
			return null;
		}

		template<typename T>
		[[nodiscard]] bool hasBasicType() const {
			return hasBasicType(TypeId::get<T>());
		}

		template<typename T>
		[[nodiscard]] const TypeReflection& getBasicType() const {
			if (const TypeId & typeId{ TypeId::get<T>() }; hasBasicType(typeId)) {
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

		template <class T>
		bool isReflected() const {
			return hasClass<T>() || hasEnum<T>() || hasBasicType<T>();
		}

		Log moveLog();

	private:
		void initializeBasicTypes();

		Log mLog;
		size_t mNextTypeId{};
		std::unordered_map<std::string, TypeId> mTypeNameLookup;
		std::unordered_map<TypeId, TypeReflection> mBasicTypeReflections;
		std::unordered_map<TypeId, ClassReflection> mClassReflections;
		std::unordered_map<TypeId, EnumReflection> mEnumReflections;
	};

	ReflectionContext& getCurrentReflectionContext() {
		static ReflectionContext reflectionContext;
		return reflectionContext;
	}

	template <typename ValueType>
	void reflectIfNotExisting(ReflectionContext& reflectionContext) {
		if (!reflectionContext.isReflected<ValueType>()) {
			reflect<ValueType>(reflectionContext);
		}
	}

	template<typename ValueType>
	void reflectIfValidType(ReflectionContext& reflectionContext) {
		if constexpr (
			std::is_enum_v<ValueType> || (std::is_class_v<ValueType> && !std::is_same_v<ValueType, std::string> &&
										  !std::is_same_v<ValueType, TypeId>) ) {

			reflectIfNotExisting<ValueType>(reflectionContext);
		}
	}

	template<class T, typename ValueType>
	void reflectIfValidType(ReflectionContext& reflectionContext, ValueType T::*) {
		reflectIfValidType<ValueType>(reflectionContext);
	}

	template<class T, typename ValueType>
	void reflectIfValidType(ReflectionContext& reflectionContext, std::vector<ValueType> T::*) {
		reflectIfValidType<ValueType>(reflectionContext);
	}

	template<class T, typename ValueType>
	void reflectIfValidType(ReflectionContext& reflectionContext, std::optional<ValueType> T::*) {
		reflectIfValidType<ValueType>(reflectionContext);
	}

	template<class T, typename KeyType, typename ValueType>
	void reflectIfValidType(ReflectionContext& reflectionContext, std::map<KeyType, ValueType> T::*) {
		reflectIfValidType<KeyType>(reflectionContext);
		reflectIfValidType<ValueType>(reflectionContext);
	}

	template<class T>
	class ClassReflectionBuilder : public ClassReflectionBuilderBase {
	public:
		ClassReflectionBuilder(ReflectionContext& context, std::string_view className)
			: ClassReflectionBuilderBase(TypeId::get<T>(), className)
			, mContext(context) {}

		template<typename Member>
		ClassReflectionBuilder& property(std::string_view name, Member&& member) {
			reflectIfValidType(mContext, member);

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

	template<typename T>
	class EnumReflectionBuilder : public EnumReflectionBuilderBase {
	public:
		EnumReflectionBuilder(ReflectionContext& context, std::string_view name)
			: EnumReflectionBuilderBase(name)
			, mContext(context) {}

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

	template<class T>
	void ClassReflectionBuilder<T>::build() {
		mContext.addClass(TypeId::get<T>(), getReflection());
	}

	template<typename T>
	void EnumReflectionBuilder<T>::build() {
		mContext.addEnum(TypeId::get<T>(), getReflection());
	}

} // namespace Core
