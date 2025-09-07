module;

#include <cassert>
#include <functional>
#include <map>
#include <utility>
#include <variant>

module Core.MapTypeTraits;

import Core.TypeTraits;

namespace Core {
	MapTypeTraits::MapTypeTraits(
		TypeId keyType, TypeId valueType, std::function<void(void*, const std::map<void*, void*>&)> applyFunc,
		std::function<void(void*, std::function<void(const void*, const void*)>)> forEachFunc)
		: keyType(std::move(keyType))
		, valueType(std::move(valueType)) {

		this->applyFunc = [applyFunc = std::move(applyFunc)](Any& dest, const std::map<Any, Any>& source) {
			const auto& typeTraits{ getTypeTraits(dest.getTypeId()) };
			if (!std::holds_alternative<MapTypeTraits>(typeTraits)) {
				return;
			}

			const auto& mapTypeTraits{ std::get<MapTypeTraits>(typeTraits) };

			std::map<void*, void*> voidMap;
			for (const auto& [inputKey, inputValue] : source) {
				assert(inputKey.getTypeId() == mapTypeTraits.keyType);
				assert(inputValue.getTypeId() == mapTypeTraits.valueType);

				voidMap[inputKey.getInstance()] = inputValue.getInstance();
			}

			applyFunc(dest.getInstance(), voidMap);
		};
		this->forEachFunc = [keyType, valueType, forEachFunc = std::move(forEachFunc)](
								const Any& mapAny, const std::function<void(const Any&, const Any&)>& visitor) {
			forEachFunc(
				mapAny.getInstance(),
				[keyType, valueType, &visitor](const void* keyInstance, const void* valueInstance) {
					visitor(Any{ keyType, keyInstance }, Any{ valueType, valueInstance });
				});
		};
	}
} // namespace Core
