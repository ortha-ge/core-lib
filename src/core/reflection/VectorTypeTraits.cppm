module;

#include <functional>
#include <vector>

export module Core.VectorTypeTraits;

import Core.Any;
import Core.BasicTypeTraits;
import Core.TypeId;

export namespace Core {
	struct VectorTypeTraits : BasicTypeTraits {
		VectorTypeTraits(
			TypeId, TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc,
			BasicTypeInnerApplyFunc, VectorTypeInnerApplyFunc, VectorTypeInnerForEachFunc);

		TypeId elementType;
		std::function<void(Any&, const std::vector<Any>&)> vectorApplyFunc;
		std::function<void(const Any&, std::function<void(const Any&)>)> vectorForEachFunc;
	};
} // namespace Core
