module;

#include <functional>

export module Core.SharedPtrTypeTraits;

import Core.Any;
import Core.BasicTypeTraits;
import Core.TypeId;

export namespace Core {
	struct SharedPtrTypeTraits : BasicTypeTraits {
		SharedPtrTypeTraits(
			TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
			OptionalTypeInnerApplyFunc, OptionalTypeInnerGetFunc);

		TypeId elementType;
		std::function<void(Any&, const Any&)> sharedPtrApplyFunc;
		std::function<Any(const Any&)> sharedPtrGetFunc;
	};
} // namespace Core
