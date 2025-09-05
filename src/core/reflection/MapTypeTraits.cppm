module;

#include <functional>
#include <map>

export module Core.MapTypeTraits;

import Core.Any;
import Core.TypeId;

export namespace Core {
	struct MapTypeTraits {
		MapTypeTraits(
			TypeId keyType, TypeId valueType, std::function<void(void*, const std::map<void*, void*>&)> applyFunc,
			std::function<void(void*, std::function<void(const void*, const void*)>)> forEachFunc);

		TypeId keyType;
		TypeId valueType;
		std::function<void(Any&, const std::map<Any, Any>&)> applyFunc;
		std::function<void(const Any&, const std::function<void(const Any&, const Any&)>&)> forEachFunc;
	};
} // namespace Core
