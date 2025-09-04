module;

#include <functional>

export module Core.BasicTypeTraits;

import Core.Any;
import Core.TypeId;

export namespace Core {

	struct BasicTypeTraits {
		BasicTypeTraits(TypeId typeId,
			std::function<void*()> constructFunc,
			std::function<void(void*)> destroyFunc,
			std::function<void(void*, const void*)> applyFunc);

		TypeId typeId;
		std::function<Any()> constructFunc;
		std::function<void(Any&)> destroyFunc;
		std::function<void(Any&, const Any&)> applyFunc;
	};
}