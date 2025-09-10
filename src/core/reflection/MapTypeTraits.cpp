module;

#include <cassert>
#include <functional>
#include <map>
#include <utility>
#include <variant>

module Core.MapTypeTraits;

import Core.TypeTraits;

namespace Core {
	MapTypeTraits::MapTypeTraits(TypeId mapType,
			TypeId keyType, TypeId valueType, BasicTypeInnerCreateFunc innerCreateFunc, BasicTypeInnerDestroyFunc innerDestroyFunc,
			BasicTypeInnerApplyFunc innerApplyFunc, MapTypeInnerApplyFunc innerMapApplyFunc, MapTypeInnerForEachFunc innerMapForEachFunc)
		: BasicTypeTraits(std::move(mapType), std::move(innerCreateFunc), std::move(innerDestroyFunc), std::move(innerApplyFunc))
		, keyType(std::move(keyType))
		, valueType(std::move(valueType)) {

		mapApplyFunc = [applyFunc = std::move(innerMapApplyFunc)](Any& dest, const std::map<Any, Any>& source) {
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

		mapForEachFunc = [keyType, valueType, forEachFunc = std::move(innerMapForEachFunc)](
								const Any& mapAny, const std::function<void(const Any&, const Any&)>& visitor) {
			forEachFunc(
				mapAny.getInstance(),
				[keyType, valueType, &visitor](const void* keyInstance, const void* valueInstance) {
					visitor(Any{ keyType, keyInstance }, Any{ valueType, valueInstance });
				});
		};
	}
} // namespace Core
