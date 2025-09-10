module;

#include <vector>

module Core.VariantTypeTraits;

namespace Core {
	VariantTypeTraits::VariantTypeTraits(
		TypeId variantType, std::vector<TypeId> types, BasicTypeInnerCreateFunc innerCreateFunc,
		BasicTypeInnerDestroyFunc innerDestroyFunc, BasicTypeInnerApplyFunc innerApplyFunc,
		VariantTypeInnerApplyFunc innerOptionalApplyFunc)
		: BasicTypeTraits(
			  std::move(variantType), std::move(innerCreateFunc), std::move(innerDestroyFunc),
			  std::move(innerApplyFunc))
		, types(std::move(types)) {

		variantApplyFunc = [applyFunc = std::move(innerOptionalApplyFunc)](Any& lhs, const Any& rhs) {
			applyFunc(lhs.getInstance(), rhs.getTypeId(), rhs.getInstance());
		};
	}

} // namespace Core
