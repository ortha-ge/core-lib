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
		TypeId keyType, TypeId valueType, BasicTypeInnerCreateFunc innerCreateFunc,
		BasicTypeInnerDestroyFunc innerDestroyFunc, BasicTypeInnerApplyFunc innerApplyFunc,
		MapTypeInnerApplyFunc innerMapApplyFunc, MapTypeInnerForEachFunc innerMapForEachFunc)
		: BasicTypeTraits(std::move(innerCreateFunc), std::move(innerDestroyFunc), std::move(innerApplyFunc))
		, keyType(std::move(keyType))
		, valueType(std::move(valueType)) {

		mapApplyFunc = [applyFunc = std::move(innerMapApplyFunc)](Any& dest, const std::map<Any, Any>& source) {
			const auto& typeTraits{ getTypeTraits(dest.getTypeId()) };
			if (!std::holds_alternative<MapTypeTraits>(typeTraits)) {
				return;
			}

			const auto& mapTypeTraits{ std::get<MapTypeTraits>(typeTraits) };

			std::vector<std::pair<TypeInstance, TypeInstance>> voidMap;
			for (const auto& [inputKey, inputValue] : source) {
				assert(inputKey.getTypeId() == mapTypeTraits.keyType);
				assert(inputValue.getTypeId() == mapTypeTraits.valueType);

				voidMap.emplace_back(inputKey.getTypeInstance(), inputValue.getTypeInstance());
			}

			applyFunc(dest.getTypeInstance(), voidMap);
		};

		mapForEachFunc = [forEachFunc = std::move(innerMapForEachFunc)](
							 const Any& mapAny, const std::function<void(const Any&, const Any&)>& visitor) {
			forEachFunc(
				mapAny.getTypeInstance(),
				[&visitor](const TypeInstance& keyInstance, const TypeInstance& valueInstance) {
					visitor(Any{ keyInstance }, Any{ valueInstance });
				});
		};
	}
} // namespace Core
