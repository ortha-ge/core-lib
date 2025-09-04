module;

#include <functional>
#include <utility>
#include <vector>

export module Core.VectorTypeTraits;

import Core.Any;
import Core.TypeId;

export namespace Core {
	struct VectorTypeTraits {
		VectorTypeTraits(TypeId elementType,
			   std::function<void(void*, const std::vector<void*>&)> applyFunc,
			   std::function<void(void*, std::function<void(const void*)>)> forEachFunc);

		TypeId elementType;
		std::function<void(Any&, const std::vector<Any>&)> applyFunc;
		std::function<void(const Any&, std::function<void(const Any&)>)> forEachFunc;
	};
}