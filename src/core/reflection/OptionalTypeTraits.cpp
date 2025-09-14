module;

#include <functional>
#include <utility>

module Core.OptionalTypeTraits;

namespace Core {
	OptionalTypeTraits::OptionalTypeTraits(
		TypeId elementType, BasicTypeInnerCreateFunc innerCreateFunc, BasicTypeInnerDestroyFunc innerDestroyFunc,
		BasicTypeInnerApplyFunc innerApplyFunc, OptionalTypeInnerApplyFunc optionalInnerApplyFunc,
		OptionalTypeInnerGetFunc optionalInnerGetFunc)
		: BasicTypeTraits(std::move(innerCreateFunc), std::move(innerDestroyFunc), std::move(innerApplyFunc))
		, elementType(std::move(elementType)) {

		optionalApplyFunc = [applyFunc = std::move(optionalInnerApplyFunc)](Any& destAny, const Any& srcAny) {
			applyFunc(destAny.getInstance(), srcAny.getInstance());
		};

		optionalGetFunc = [elementType, getFunc = std::move(optionalInnerGetFunc)](const Any& any) -> Any {
			return { elementType, getFunc(any.getInstance()) };
		};
	}
} // namespace Core
