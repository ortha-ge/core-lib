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
	struct TypeInstance;

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

	using OptionalTypeInnerApplyFunc = std::function<void(void*, const void*)>;
	using OptionalTypeInnerGetFunc = std::function<void*(void*)>;

	using VectorTypeInnerApplyFunc = std::function<void(void*, const std::vector<void*>&)>;
	using VectorTypeInnerForEachFunc = std::function<void(void*, std::function<void(const void*)>)>;

	using MapTypeInnerApplyFunc = std::function<void(void*, const std::map<void*, void*>&)>;
	using MapTypeInnerForEachFunc = std::function<void(void*, std::function<void(const void*, const void*)>)>;

	using VariantTypeElementInnerApplyFunc = std::function<void(void*, const void*)>;
	using VariantTypeInnerApplyFunc = std::function<void(void*, const TypeId&, const void*)>;

}

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
		size_t, TypeId, TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc,
		BasicTypeInnerApplyFunc, MapTypeInnerApplyFunc, MapTypeInnerForEachFunc);

	std::unique_ptr<TypeInfo> createVariantTypeInfo(size_t,
		std::vector<TypeId>, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
		VariantTypeInnerApplyFunc applyFunc);
} // namespace Core

export namespace Core {

	class TypeId {
	public:
		TypeId();
		TypeId(const TypeInfo& typeIdInfo);
		~TypeId();

		TypeId(const TypeId& other);
		TypeId(TypeId&&) noexcept;

		TypeId& operator=(const TypeId& other);
		TypeId& operator=(TypeId&&) noexcept;

		bool operator==(const TypeId& other) const;
		bool operator==(const TypeInfo& other) const;
		bool operator==(const TypeInfo* other) const;

		const TypeInfo& getTypeInfo() const;

		template<typename T, std::enable_if_t<std::is_void_v<T>, bool> = true>
		static TypeId _get() {
			static auto typeInfo{ createVoidTypeInfo() };
			return { *typeInfo };
		}

		template <typename ValueType>
		static auto _getBasicTypeCreateFuncs();

		template<typename ValueType, std::enable_if_t<!std::is_void_v<ValueType>, bool> = true>
		static TypeId _get() {
			auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<ValueType>();
			static auto typeInfo{ createBasicTypeInfo(sizeof(ValueType), createFunc, destroyFunc, applyFunc) };
			return { *typeInfo };
		}

		template<typename ValueType>
		static TypeId get(const ValueType*) {
			return TypeId::_get<ValueType>();
		}

		template<typename ValueType>
		static TypeId get(const std::optional<ValueType>*) {
			auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<std::optional<ValueType>>();
			auto optionalApplyFunc = [](void* optionalInstance, const void* wrappedTypeInstance) {
				auto& optional = *static_cast<std::optional<ValueType>*>(optionalInstance);
				auto& wrappedType = *static_cast<const ValueType*>(wrappedTypeInstance);
				optional = wrappedType;
			};

			auto optionalGetFunc = [](void* optionalInstance) -> void* {
				auto& optional = *static_cast<std::optional<ValueType>*>(optionalInstance);
				return optional.has_value() ? &optional.value() : nullptr;
			};

			static auto typeInfo{ createOptionalTypeInfo(
				sizeof(std::optional<ValueType>), TypeId::get<ValueType>(), createFunc, destroyFunc, applyFunc,
				optionalApplyFunc, optionalGetFunc) };

			return { *typeInfo };
		}

		template<typename Variant, typename ValueType>
		static VariantTypeElementInnerApplyFunc createVariantApplyFunc() {
			return [](void* variantInstance, const void* rhs) {
				auto& variant = *static_cast<Variant*>(variantInstance);
				variant = *static_cast<const ValueType*>(rhs);
			};
		}

		template<typename... ValueTypes>
		static TypeId get(const std::variant<ValueTypes...>*) {
			auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<std::variant<ValueTypes...>>();

			std::unordered_map<TypeId, VariantTypeElementInnerApplyFunc, TypeIdHasher> applyFuncs = {
				{ TypeId::get<ValueTypes>(), createVariantApplyFunc<std::variant<ValueTypes...>, ValueTypes>() }...
			};

			auto variantApplyFunc = [applyFuncs = std::move(applyFuncs)](
										void* variantInstance, const TypeId& rhsTypeId, const void* rhsInstance) {
				if (!applyFuncs.contains(rhsTypeId)) {
					assert(false);
				}

				auto& applyFunc = applyFuncs.at(rhsTypeId);
				applyFunc(variantInstance, rhsInstance);
			};

			std::vector<TypeId> types{ TypeId::get<ValueTypes>()... };
			static auto typeInfo{ createVariantTypeInfo(sizeof(std::variant<ValueTypes...>),
				std::move(types), std::move(createFunc), std::move(destroyFunc), std::move(applyFunc),
				std::move(variantApplyFunc)) };
			return { *typeInfo };
		}

		template<typename ValueType>
		static TypeId get(const std::vector<ValueType>*) {
			auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<std::vector<ValueType>>();

			auto vectorApplyFunc = [](void* vectorInstance, const std::vector<void*>& inputVector) {
				auto& vector = *static_cast<std::vector<ValueType>*>(vectorInstance);
				vector = {};

				for (const auto& inputElement : inputVector) {
					auto& wrappedType = *static_cast<const ValueType*>(inputElement);
					vector.emplace_back(wrappedType);
				}
			};

			auto vectorForEachFunc = [](void* vectorInstance, std::function<void(const void*)> visitor) {
				auto& vector = *static_cast<std::vector<ValueType>*>(vectorInstance);
				for (const auto& outputElement : vector) {
					visitor(&outputElement);
				}
			};
			static auto typeInfo{ createVectorTypeInfo(
				sizeof(std::vector<ValueType>), TypeId::get<ValueType>(), createFunc, destroyFunc, applyFunc,
				vectorApplyFunc, vectorForEachFunc) };
			return { *typeInfo };
		}

		template<typename KeyType, typename ValueType>
		static TypeId get(const std::map<KeyType, ValueType>*) {
			auto [createFunc, destroyFunc, applyFunc] = _getBasicTypeCreateFuncs<std::map<KeyType, ValueType>>();

			auto mapApplyFunc = [](void* mapInstance, const std::map<void*, void*>& inputMap) {
				auto& map = *static_cast<std::map<KeyType, ValueType>*>(mapInstance);
				map = {};

				for (const auto& [inputKey, inputValue] : inputMap) {
					auto& wrappedKeyType = *static_cast<const KeyType*>(inputKey);
					auto& wrappedValueType = *static_cast<const ValueType*>(inputValue);
					map[wrappedKeyType] = wrappedValueType;
				}
			};

			auto mapForEachFunc = [](void* mapInstance, std::function<void(const void*, const void*)> visitor) {
				auto& map = *static_cast<std::map<KeyType, ValueType>*>(mapInstance);
				for (const auto& [outputKey, outputValue] : map) {
					visitor(&outputKey, &outputValue);
				}
			};

			static auto typeInfo{ createMapTypeInfo(
				sizeof(std::map<KeyType, ValueType>), TypeId::get<KeyType>(), TypeId::get<ValueType>(), createFunc,
				destroyFunc, applyFunc, mapApplyFunc, mapForEachFunc) };
			return { *typeInfo };
		}

		template <typename ValueType>
		static TypeId get() {
			const ValueType* nullInstance = nullptr;
			return get(nullInstance);
		}

		template<class Class, typename ValueType>
		static TypeId get(ValueType Class::*) {
			return get<ValueType>();
		}

	private:
		const TypeInfo* mTypeInfo{ nullptr };
	};

	/* @brief Wrapper for a void* and it's associated TypeId.
	 * Safer than just passing void* since you can check the type before static_cast.
	 */
	struct TypeInstance {
		TypeId typeId;
		void* instance{ nullptr };
	};

	template <typename ValueType>
	auto TypeId::_getBasicTypeCreateFuncs() {
		auto createFunc = []() {
			return TypeInstance{ TypeId::get<ValueType>(), new ValueType() };
		};
		auto destroyFunc = [](TypeInstance& typeInstance) {
			if (TypeId::get<ValueType>() != typeInstance.typeId) {
				return;
			}

			delete static_cast<ValueType*>(typeInstance.instance);
		};
		auto applyFunc = [](TypeInstance& destInstance, const TypeInstance& sourceInstance) {
			const auto& typeId(TypeId::get<ValueType>());
			const bool isDestTypeValid = destInstance.typeId == typeId;
			const bool isSourceTypeValid = sourceInstance.typeId == typeId;

			assert(isDestTypeValid && isSourceTypeValid);
			if (!isDestTypeValid || !isSourceTypeValid) {
				return;
			}

			ValueType& dest{ *static_cast<ValueType*>(destInstance.instance) };
			dest = *static_cast<const ValueType*>(sourceInstance.instance);
		};

		return BasicTypeInnerFuncs(std::move(createFunc), std::move(destroyFunc), std::move(applyFunc));
	}

} // namespace Core
