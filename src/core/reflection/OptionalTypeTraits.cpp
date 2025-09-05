module;

#include <functional>
#include <utility>

module Core.OptionalTypeTraits;

namespace Core {
	OptionalTypeTraits::OptionalTypeTraits(
		TypeId elementType, std::function<void(void*, const void*)> applyFunc, std::function<void*(void*)> getFunc)
		: elementType(std::move(elementType)) {

		this->applyFunc = [applyFunc = std::move(applyFunc)](Any& destAny, const Any& srcAny) {
			applyFunc(destAny.getInstance(), srcAny.getInstance());
		};
		this->getFunc = [elementType, getFunc = std::move(getFunc)](const Any& any) -> Any {
			return { elementType, getFunc(any.getInstance()) };
		};
	}
} // namespace Core
