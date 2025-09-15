module;

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

export module Core.TypeId;

import Core.TypeInfo;

export namespace Core {

	class TypeId;
	class TypeInstance;

	struct TypeIdHasher {
		std::size_t operator()(const TypeId& typeId) const noexcept;
	};

	using BasicTypeInnerCreateFunc = std::function<TypeInstance()>;
	using BasicTypeInnerDestroyFunc = std::function<void(TypeInstance&)>;
	using BasicTypeInnerApplyFunc = std::function<void(TypeInstance&, const TypeInstance&)>;

	struct BasicTypeInnerFuncs {
		BasicTypeInnerCreateFunc createFunc;
		BasicTypeInnerDestroyFunc destroyFunc;
		BasicTypeInnerApplyFunc applyFunc;
	};

	using OptionalTypeInnerApplyFunc = std::function<void(TypeInstance&, const TypeInstance&)>;
	using OptionalTypeInnerGetFunc = std::function<TypeInstance(const TypeInstance&)>;

	using VectorTypeInnerApplyFunc = std::function<void(TypeInstance&, const std::vector<TypeInstance>&)>;
	using VectorTypeInnerForEachFunc = std::function<void(const TypeInstance&, const std::function<void(const TypeInstance&)>&)>;

	using MapTypeInnerApplyFunc = std::function<void(TypeInstance&, const std::vector<std::pair<TypeInstance, TypeInstance>>&)>;
	using MapTypeInnerForEachFunc = std::function<void(const TypeInstance&, const std::function<void(const TypeInstance&, const TypeInstance&)>&)>;

	using VariantTypeElementInnerApplyFunc = std::function<void(TypeInstance&, const TypeInstance&)>;
	using VariantTypeInnerApplyFunc = std::function<void(TypeInstance&, const TypeInstance&)>;

} // namespace Core

namespace Core {
	std::unique_ptr<TypeInfo> createVoidTypeInfo();

	std::unique_ptr<TypeInfo>
	createBasicTypeInfo(size_t, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc);

	std::unique_ptr<TypeInfo> createOptionalTypeInfo(
		size_t, TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
		OptionalTypeInnerApplyFunc, OptionalTypeInnerGetFunc);

	std::unique_ptr<TypeInfo> createVectorTypeInfo(
		size_t, TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
		VectorTypeInnerApplyFunc, VectorTypeInnerForEachFunc);

	std::unique_ptr<TypeInfo> createMapTypeInfo(
		size_t, TypeId, TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
		MapTypeInnerApplyFunc, MapTypeInnerForEachFunc);

	std::unique_ptr<TypeInfo> createVariantTypeInfo(
		size_t, std::vector<TypeId>, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
		VariantTypeInnerApplyFunc applyFunc);

	template<typename Variant, typename ValueType>
	VariantTypeElementInnerApplyFunc createVariantElementApplyFunc();

} // namespace Core

export namespace Core {

	class TypeId {
	public:
		TypeId();
		explicit TypeId(const TypeInfo& typeIdInfo);
		~TypeId();

		TypeId(const TypeId& other);
		TypeId(TypeId&&) noexcept;

		TypeId& operator=(const TypeId& other);
		TypeId& operator=(TypeId&&) noexcept;

		bool operator==(const TypeId& other) const;
		bool operator==(const TypeInfo& other) const;
		bool operator==(const TypeInfo* other) const;

		[[nodiscard]] const TypeInfo& getTypeInfo() const;

		template<typename ValueType>
		static auto _getBasicTypeCreateFuncs();

		template<typename T, std::enable_if_t<std::is_void_v<T>, bool> = true>
		static TypeId _get();

		template<typename ValueType, std::enable_if_t<!std::is_void_v<ValueType>, bool> = true>
		static TypeId _get();

		template<typename ValueType>
		static TypeId get(const ValueType*);

		template<typename KeyType, typename ValueType>
		static TypeId get(const std::map<KeyType, ValueType>*);

		template<typename ValueType>
		static TypeId get(const std::optional<ValueType>*);

		template<typename... ValueTypes>
		static TypeId get(const std::variant<ValueTypes...>*);

		template<typename ValueType>
		static TypeId get(const std::vector<ValueType>*);

		template<typename ValueType>
		static TypeId get();

		template<class Class, typename ValueType>
		static TypeId get(ValueType Class::*);

	private:
		const TypeInfo* mTypeInfo{ nullptr };
	};

	/* @brief Wrapper for a void* and it's associated TypeId.
	 * Safer than just passing void* since you can check the type before static_cast.
	 */
	class TypeInstance {
	public:

		TypeInstance();
		TypeInstance(TypeId typeId, void* instance, bool isConst);
		TypeInstance(TypeId typeId, void* instance);
		TypeInstance(TypeId typeId, const void* instance);
		TypeInstance(TypeId typeId, std::nullptr_t);

		const TypeId& getTypeId() const;
		void* getInstance() const;
		bool getIsConst() const;

	private:

		TypeId mTypeId;
		void* mInstance{ nullptr };
		bool mIsConst{ false };
	};

	template<typename T, std::enable_if_t<std::is_void_v<T>, bool>>
	TypeId TypeId::_get() {
		static auto typeInfo{ createVoidTypeInfo() };
		return TypeId{ *typeInfo };
	}

	template<typename ValueType, std::enable_if_t<!std::is_void_v<ValueType>, bool>>
	TypeId TypeId::_get() {
		auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<ValueType>();
		static auto typeInfo{ createBasicTypeInfo(sizeof(ValueType), createFunc, destroyFunc, applyFunc) };
		return TypeId{ *typeInfo };
	}

	template<typename ValueType>
	TypeId TypeId::get(const ValueType*) {
		return TypeId::_get<ValueType>();
	}

	template<typename ValueType>
	auto TypeId::_getBasicTypeCreateFuncs() {
		auto createFunc = []() { return TypeInstance{ TypeId::get<ValueType>(), new ValueType() }; };
		auto destroyFunc = [](TypeInstance& typeInstance) {
			if (TypeId::get<ValueType>() != typeInstance.getTypeId()) {
				return;
			}

			delete static_cast<ValueType*>(typeInstance.getInstance());
		};
		auto applyFunc = [](TypeInstance& destInstance, const TypeInstance& sourceInstance) {
			const auto& typeId(TypeId::get<ValueType>());
			const bool isDestTypeValid = destInstance.getTypeId() == typeId;
			const bool isSourceTypeValid = sourceInstance.getTypeId() == typeId;

			assert(isDestTypeValid && isSourceTypeValid);
			if (!isDestTypeValid || !isSourceTypeValid) {
				return;
			}

			ValueType& dest{ *static_cast<ValueType*>(destInstance.getInstance()) };
			dest = *static_cast<const ValueType*>(sourceInstance.getInstance());
		};

		return BasicTypeInnerFuncs(std::move(createFunc), std::move(destroyFunc), std::move(applyFunc));
	}

	template<typename KeyType, typename ValueType>
	TypeId TypeId::get(const std::map<KeyType, ValueType>*) {
		using MapType = std::map<KeyType, ValueType>;
		auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<MapType>();

		auto mapApplyFunc = [](const TypeInstance& mapInstance, const std::vector<std::pair<TypeInstance, TypeInstance>>& inputMap) {
			const bool isMapInstanceTypeValid = mapInstance.getTypeId() == TypeId::get<MapType>();
			assert(isMapInstanceTypeValid);
			if (!isMapInstanceTypeValid) {
				return;
			}

			MapType newMap;
			for (const auto& [inputKey, inputValue] : inputMap) {
				const bool isKeyTypeValid = inputKey.getTypeId() == TypeId::get<KeyType>();
				const bool isValueTypeValid = inputValue.getTypeId() == TypeId::get<ValueType>();
				assert(isKeyTypeValid && isValueTypeValid);
				if (!isKeyTypeValid || !isValueTypeValid) {
					return;
				}

				auto& wrappedKeyType = *static_cast<const KeyType*>(inputKey.getInstance());
				auto& wrappedValueType = *static_cast<const ValueType*>(inputValue.getInstance());
				newMap[wrappedKeyType] = wrappedValueType;
			}

			*static_cast<MapType*>(mapInstance.getInstance()) = std::move(newMap);
		};

		auto mapForEachFunc = [](const TypeInstance& mapInstance, const std::function<void(const TypeInstance&, const TypeInstance&)>& visitor) {
			const bool isMapInstanceTypeValid = mapInstance.getTypeId() == TypeId::get<MapType>();
			assert(isMapInstanceTypeValid);
			if (!isMapInstanceTypeValid) {
				return;
			}

			const auto keyType = TypeId::get<KeyType>();
			const auto valueType = TypeId::get<ValueType>();
			const auto& map = *static_cast<std::map<KeyType, ValueType>*>(mapInstance.getInstance());

			for (auto&& [outputKey, outputValue] : map) {
				visitor(TypeInstance(keyType, &outputKey), TypeInstance(valueType, &outputValue));
			}
		};

		static auto typeInfo{ createMapTypeInfo(
			sizeof(std::map<KeyType, ValueType>), TypeId::get<KeyType>(), TypeId::get<ValueType>(), createFunc,
			destroyFunc, applyFunc, mapApplyFunc, mapForEachFunc) };
		return TypeId{ *typeInfo };
	}

	template<typename ValueType>
	TypeId TypeId::get(const std::optional<ValueType>*) {
		using OptionalType = std::optional<ValueType>;
		auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<OptionalType>();
		auto optionalApplyFunc = [](TypeInstance& optionalInstance, const TypeInstance& wrappedTypeInstance) {

			const bool isOptionalTypeValid = optionalInstance.getTypeId() == TypeId::get<OptionalType>();
			const bool isWrappedTypeValid = wrappedTypeInstance.getTypeId() == TypeId::get<ValueType>();
			assert(isOptionalTypeValid && isWrappedTypeValid);
			if (!isOptionalTypeValid || !isWrappedTypeValid) {
				return;
			}

			auto& optional = *static_cast<OptionalType*>(optionalInstance.getInstance());
			auto& wrappedType = *static_cast<const ValueType*>(wrappedTypeInstance.getInstance());
			optional = wrappedType;
		};

		auto optionalGetFunc = [](const TypeInstance& optionalInstance) -> TypeInstance {
			const bool isOptionalTypeValid = optionalInstance.getTypeId() == TypeId::get<OptionalType>();
			assert(isOptionalTypeValid);
			if (!isOptionalTypeValid) {
				return {};
			}

			if (auto& optional = *static_cast<OptionalType*>(optionalInstance.getInstance())) {
				return TypeInstance{ TypeId::get<ValueType>(), &optional.value() };
			}

			return {};
		};

		static auto typeInfo{ createOptionalTypeInfo(
			sizeof(std::optional<ValueType>), TypeId::get<ValueType>(), createFunc, destroyFunc, applyFunc,
			optionalApplyFunc, optionalGetFunc) };

		return TypeId{ *typeInfo };
	}

	template<typename... ValueTypes>
	TypeId TypeId::get(const std::variant<ValueTypes...>*) {
		using VariantType = std::variant<ValueTypes...>;
		auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<VariantType>();

		std::unordered_map<TypeId, VariantTypeElementInnerApplyFunc, TypeIdHasher> applyFuncs = {
			{ TypeId::get<ValueTypes>(), createVariantElementApplyFunc<VariantType, ValueTypes>() }...
		};

		auto variantApplyFunc = [applyFuncs = std::move(applyFuncs)](
									TypeInstance& variantInstance, const TypeInstance& rhsInstance) {
			const bool isVariantInstanceTypeValid = variantInstance.getTypeId() == TypeId::get<VariantType>();
			assert(isVariantInstanceTypeValid);
			if (!isVariantInstanceTypeValid) {
				return;
			}

			const bool isVariantTypeValid = applyFuncs.contains(rhsInstance.getTypeId());
			assert(isVariantTypeValid);
			if (!isVariantTypeValid) {
				return;
			}

			const auto& applyFunc = applyFuncs.at(rhsInstance.getTypeId());
			applyFunc(variantInstance, rhsInstance);
		};

		std::vector<TypeId> types{ TypeId::get<ValueTypes>()... };
		static auto typeInfo{ createVariantTypeInfo(
			sizeof(std::variant<ValueTypes...>), std::move(types), std::move(createFunc), std::move(destroyFunc),
			std::move(applyFunc), std::move(variantApplyFunc)) };
		return TypeId{ *typeInfo };
	}

	template<typename ValueType>
	TypeId TypeId::get(const std::vector<ValueType>*) {
		using VectorType = std::vector<ValueType>;
		auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<VectorType>();

		auto vectorApplyFunc = [](const TypeInstance& vectorInstance, const std::vector<TypeInstance>& inputVector) {
			const bool isVectorInstanceTypeValid = vectorInstance.getTypeId() == TypeId::get<VectorType>();
			assert(isVectorInstanceTypeValid);
			if (!isVectorInstanceTypeValid) {
				return;
			}

			VectorType newVector;
			for (const auto& inputElement : inputVector) {
				const bool isVectorElementTypeValid = inputElement.getTypeId() == TypeId::get<ValueType>();
				assert(isVectorElementTypeValid);
				if (!isVectorElementTypeValid) {
					return;
				}

				auto& wrappedType = *static_cast<const ValueType*>(inputElement.getInstance());
				newVector.emplace_back(wrappedType);
			}

			*static_cast<VectorType*>(vectorInstance.getInstance()) = std::move(newVector);
		};

		auto vectorForEachFunc = [](const TypeInstance& vectorInstance, const std::function<void(const TypeInstance&)>& visitor) {
			const bool isVectorInstanceTypeValid = vectorInstance.getTypeId() == TypeId::get<VectorType>();
			assert(isVectorInstanceTypeValid);
			if (!isVectorInstanceTypeValid) {
				return;
			}

			auto& vector = *static_cast<VectorType*>(vectorInstance.getInstance());
			for (auto& outputElement : vector) {
				visitor(TypeInstance(TypeId::get<ValueType>(), &outputElement));
			}
		};
		static auto typeInfo{ createVectorTypeInfo(
			sizeof(std::vector<ValueType>), TypeId::get<ValueType>(), createFunc, destroyFunc, applyFunc,
			vectorApplyFunc, vectorForEachFunc) };
		return TypeId{ *typeInfo };
	}

	template<typename ValueType>
	TypeId TypeId::get() {
		const ValueType* nullInstance = nullptr;
		return get(nullInstance);
	}

	template<class Class, typename ValueType>
	TypeId TypeId::get(ValueType Class::*) {
		return get<ValueType>();
	}

} // namespace Core

namespace Core {
	template<typename Variant, typename ValueType>
	VariantTypeElementInnerApplyFunc createVariantElementApplyFunc() {
		return [](TypeInstance& variantInstance, const TypeInstance& rhs) {
			const bool isVariantInstanceTypeValid = variantInstance.getTypeId() == TypeId::get<Variant>();
			assert(isVariantInstanceTypeValid);
			if (!isVariantInstanceTypeValid) {
				return;
			}

			const bool isRhsTypeValid = rhs.getTypeId() == TypeId::get<ValueType>();
			assert(isRhsTypeValid);
			if (!isRhsTypeValid) {
				return;
			}

			auto& variant = *static_cast<Variant*>(variantInstance.getInstance());
			variant = *static_cast<const ValueType*>(rhs.getInstance());
		};
	}
}