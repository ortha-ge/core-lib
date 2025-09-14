module;

#include <functional>
#include <utility>

module Core.BasicTypeTraits;

namespace Core {

	BasicTypeTraits::BasicTypeTraits(
		BasicTypeInnerCreateFunc constructFunc, BasicTypeInnerDestroyFunc destroyFunc,
		BasicTypeInnerApplyFunc applyFunc) {

		this->constructFunc = [constructFunc = std::move(constructFunc)]() { return Any(constructFunc(), true); };
		this->destroyFunc = [destroyFunc = std::move(destroyFunc)](Any& any) { destroyFunc(any.getTypeInstance()); };
		this->applyFunc = [applyFunc = std::move(applyFunc)](Any& destAny, const Any& srcAny) {
			applyFunc(destAny.getTypeInstance(), srcAny.getTypeInstance());
		};
	}
} // namespace Core
