module;

#include <functional>
#include <vector>

export module Core.VariantTypeTraits;

import Core.Any;
import Core.BasicTypeTraits;
import Core.TypeId;

export namespace Core {
	struct VariantTypeTraits : BasicTypeTraits {
		VariantTypeTraits(TypeId,
			std::vector<TypeId>, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
			VariantTypeInnerApplyFunc);

		std::vector<TypeId> types;
		std::function<void(Any&, const Any&)> variantApplyFunc;
	};
} // namespace Core
