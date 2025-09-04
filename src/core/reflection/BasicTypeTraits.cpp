module;

#include <functional>
#include <utility>

module Core.BasicTypeTraits;

namespace Core {

	BasicTypeTraits::BasicTypeTraits(TypeId typeId,
			std::function<void*()> constructFunc,
			std::function<void(void*)> destroyFunc,
			std::function<void(void*, const void*)> applyFunc)
			: typeId(std::move(typeId)) {

		this->constructFunc = [typeId, constructFunc = std::move(constructFunc)]() {
			return Any(typeId, constructFunc());
		};
		this->destroyFunc = [typeId, destroyFunc = std::move(destroyFunc)](Any& any) {
			destroyFunc(any.getInstance());
		};
		this->applyFunc = [typeId, applyFunc = std::move(applyFunc)](Any& destAny, const Any& srcAny) {
			applyFunc(destAny.getInstance(), srcAny.getInstance());
		};
	}
}