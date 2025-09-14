module;

#include <functional>

export module Core.BasicTypeTraits;

import Core.Any;
import Core.TypeId;

export namespace Core {

	struct BasicTypeTraits {
		BasicTypeTraits(
			BasicTypeInnerCreateFunc constructFunc, BasicTypeInnerDestroyFunc destroyFunc,
			BasicTypeInnerApplyFunc applyFunc);

		std::function<Any()> constructFunc;
		std::function<void(Any&)> destroyFunc;
		std::function<void(Any&, const Any&)> applyFunc;
	};
} // namespace Core
