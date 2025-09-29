module;

#include <functional>
#include <utility>

module Core.SharedPtrTypeTraits;

namespace Core {
	SharedPtrTypeTraits::SharedPtrTypeTraits(
		TypeId elementType, BasicTypeInnerCreateFunc innerCreateFunc, BasicTypeInnerDestroyFunc innerDestroyFunc,
		BasicTypeInnerApplyFunc innerApplyFunc, SharedPtrTypeInnerApplyFunc sharedPtrInnerApplyFunc,
		SharedPtrTypeInnerGetFunc sharedPtrInnerGetFunc)
		: BasicTypeTraits(std::move(innerCreateFunc), std::move(innerDestroyFunc), std::move(innerApplyFunc))
		, elementType(std::move(elementType)) {

		sharedPtrApplyFunc = [applyFunc = std::move(sharedPtrInnerApplyFunc)](Any& destAny, const Any& srcAny) {
			applyFunc(destAny.getTypeInstance(), srcAny.getTypeInstance());
		};

		sharedPtrGetFunc = [elementType, getFunc = std::move(sharedPtrInnerGetFunc)](const Any& any) -> Any {
			return Any{ getFunc(any.getTypeInstance()) };
		};
	}
} // namespace Core
