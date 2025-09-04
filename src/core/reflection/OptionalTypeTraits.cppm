module;

#include <functional>

export module Core.OptionalTypeTraits;

import Core.Any;
import Core.TypeId;

export namespace Core {
	struct OptionalTypeTraits {
		OptionalTypeTraits(TypeId elementType,
			 std::function<void(void*, const void*)> applyFunc,
			 std::function<void*(void*)> getFunc);

		TypeId elementType;
		std::function<void(Any&, const Any&)> applyFunc;
		std::function<Any(const Any&)> getFunc;
	};
}