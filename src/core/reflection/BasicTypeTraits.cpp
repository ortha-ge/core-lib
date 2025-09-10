module;

#include <functional>
#include <utility>

module Core.BasicTypeTraits;

namespace Core {

	BasicTypeTraits::BasicTypeTraits(
		TypeId typeId, std::function<void*()> constructFunc, std::function<void(void*)> destroyFunc,
		std::function<void(void*, const void*)> applyFunc)
		: typeId(typeId) {

		this->constructFunc = [typeId, constructFunc = std::move(constructFunc)]() {
			auto* instance = constructFunc();
			return Any(typeId, instance, true);
		};
		this->destroyFunc = [typeId, destroyFunc = std::move(destroyFunc)](Any& any) {
			destroyFunc(any.getInstance());
		};
		this->applyFunc = [typeId, applyFunc = std::move(applyFunc)](Any& destAny, const Any& srcAny) {
			applyFunc(destAny.getInstance(), srcAny.getInstance());
		};
	}
} // namespace Core
