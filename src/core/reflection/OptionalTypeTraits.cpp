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
			applyFunc(destAny.getTypeInstance(), srcAny.getTypeInstance());
		};

		optionalGetFunc = [elementType, getFunc = std::move(optionalInnerGetFunc)](const Any& any) -> Any {
			return Any{ getFunc(any.getTypeInstance()) };
		};
	}
} // namespace Core
