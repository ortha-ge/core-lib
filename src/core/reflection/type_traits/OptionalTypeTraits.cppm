module;

#include <functional>

export module Core.OptionalTypeTraits;

import Core.Any;
import Core.BasicTypeTraits;
import Core.TypeId;

export namespace Core {
	struct OptionalTypeTraits : BasicTypeTraits {
		OptionalTypeTraits(
			TypeId, BasicTypeInnerCreateFunc, BasicTypeInnerDestroyFunc, BasicTypeInnerApplyFunc,
			OptionalTypeInnerApplyFunc, OptionalTypeInnerGetFunc);

		TypeId elementType;
		std::function<void(Any&, const Any&)> optionalApplyFunc;
		std::function<Any(const Any&)> optionalGetFunc;
	};
} // namespace Core
