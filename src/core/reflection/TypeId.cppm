module;

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <vector>

export module Core.TypeId;

import Core.TypeInfo;

export namespace Core {
	class TypeId;
}

namespace Core {
	std::unique_ptr<TypeInfo> createVoidTypeInfo();

	std::unique_ptr<TypeInfo> createBasicTypeInfo(
		size_t size, std::function<void*()> createFunc, std::function<void(void*)> destroyFunc,
		std::function<void(void*, const void*)> applyFunc);

	std::unique_ptr<TypeInfo> createOptionalTypeInfo(
		size_t size, TypeId valueType, std::function<void(void*, const void*)> applyFunc,
		std::function<void*(void*)> getFunc);

	std::unique_ptr<TypeInfo> createVectorTypeInfo(
		size_t size, TypeId valueType, std::function<void(void*, const std::vector<void*>&)> applyFunc,
		std::function<void(void*, std::function<void(const void*)>)> forEachFunc);

	std::unique_ptr<TypeInfo> createMapTypeInfo(
		size_t size, TypeId keyType, TypeId valueType,
		std::function<void(void*, const std::map<void*, void*>&)> applyFunc,
		std::function<void(void*, std::function<void(const void*, const void*)>)> forEachFunc);
} // namespace Core

template<>
struct std::hash<Core::TypeId> {
	size_t operator()(const Core::TypeId& typeId) const noexcept;
};

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
		static TypeId get() {
			static auto typeInfo{ createVoidTypeInfo() };
			return { *typeInfo };
		}

		template<typename ValueType, std::enable_if_t<!std::is_void_v<ValueType>, bool> = true>
		static TypeId get() {
			auto createFunc = []() { return new ValueType(); };

			auto destroyFunc = [](void* instance) { delete static_cast<ValueType*>(instance); };

			auto applyFunc = [](void* destInstance, const void* sourceInstance) {
				ValueType& dest{ *static_cast<ValueType*>(destInstance) };
				dest = *static_cast<const ValueType*>(sourceInstance);
			};

			static auto typeInfo{ createBasicTypeInfo(sizeof(ValueType), createFunc, destroyFunc, applyFunc) };
			return { *typeInfo };
		}

		template<class Class, typename ValueType>
		static TypeId get(ValueType Class::* field) {
			return get<ValueType>();
		}

		template<class Class, typename ValueType>
		static TypeId get(std::optional<ValueType> Class::* field) {
			auto applyFunc = [](void* optionalInstance, const void* wrappedTypeInstance) {
				auto& optional = *static_cast<std::optional<ValueType>*>(optionalInstance);
				auto& wrappedType = *static_cast<const ValueType*>(wrappedTypeInstance);
				optional = wrappedType;
			};

			auto getFunc = [](void* optionalInstance) -> void* {
				auto& optional = *static_cast<std::optional<ValueType>*>(optionalInstance);
				return optional.has_value() ? &optional.value() : nullptr;
			};

			static auto typeInfo{ createOptionalTypeInfo(
				sizeof(std::optional<ValueType>), TypeId::get<ValueType>(), applyFunc, getFunc) };

			return { *typeInfo };
		}

		template<class Class, typename ValueType>
		static TypeId get(std::vector<ValueType> Class::* field) {
			auto applyFunc = [](void* vectorInstance, const std::vector<void*>& inputVector) {
				auto& vector = *static_cast<std::vector<ValueType>*>(vectorInstance);
				vector = {};

				for (const auto& inputElement : inputVector) {
					auto& wrappedType = *static_cast<const ValueType*>(inputElement);
					vector.emplace_back(wrappedType);
				}
			};

			auto forEachFunc = [](void* vectorInstance, std::function<void(const void*)> visitor) {
				auto& vector = *static_cast<std::vector<ValueType>*>(vectorInstance);
				for (const auto& outputElement : vector) {
					visitor(&outputElement);
				}
			};
			static auto typeInfo{ createVectorTypeInfo(
				sizeof(std::vector<ValueType>), TypeId::get<ValueType>(), applyFunc, forEachFunc) };
			return { *typeInfo };
		}

		template<class Class, typename KeyType, typename ValueType>
		static TypeId get(std::map<KeyType, ValueType> Class::* field) {


			auto applyFunc = [](void* mapInstance, const std::map<void*, void*>& inputMap) {
				auto& map = *static_cast<std::map<KeyType, ValueType>*>(mapInstance);
				map = {};

				for (const auto& [inputKey, inputValue] : inputMap) {
					auto& wrappedKeyType = *static_cast<const KeyType*>(inputKey);
					auto& wrappedValueType = *static_cast<const ValueType*>(inputValue);
					map[wrappedKeyType] = wrappedValueType;
				}
			};

			auto forEachFunc = [](void* mapInstance, std::function<void(const void*, const void*)> visitor) {
				auto& map = *static_cast<std::map<KeyType, ValueType>*>(mapInstance);
				for (const auto& [outputKey, outputValue] : map) {
					visitor(&outputKey, &outputValue);
				}
			};

			static auto typeInfo{ createMapTypeInfo(
				sizeof(std::map<KeyType, ValueType>), TypeId::get<KeyType>(), TypeId::get<ValueType>(), applyFunc,
				forEachFunc) };
			return { *typeInfo };
		}

	private:
		const TypeInfo* mTypeInfo{ nullptr };
	};

} // namespace Core
