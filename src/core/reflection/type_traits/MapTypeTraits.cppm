module;

#include <functional>
#include <map>

export module Core.MapTypeTraits;

import Core.Any;
import Core.BasicTypeTraits;
import Core.TypeId;

export namespace Core {
	struct MapTypeTraits : BasicTypeTraits {
		MapTypeTraits(
			TypeId, TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
			MapTypeInnerApplyFunc, MapTypeInnerForEachFunc);

		TypeId keyType;
		TypeId valueType;
		std::function<void(Any&, const std::map<Any, Any>&)> mapApplyFunc;
		std::function<void(const Any&, const std::function<void(const Any&, const Any&)>&)> mapForEachFunc;
	};
} // namespace Core
