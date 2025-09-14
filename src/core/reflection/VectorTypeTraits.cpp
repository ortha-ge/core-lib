module;

#include <functional>
#include <variant>

module Core.VectorTypeTraits;

import Core.Any;
import Core.TypeId;
import Core.TypeTraits;

namespace Core {
	VectorTypeTraits::VectorTypeTraits(
		TypeId elementType, BasicTypeInnerCreateFunc innerCreateFunc, BasicTypeInnerDestroyFunc innerDestroyFunc,
		BasicTypeInnerApplyFunc innerApplyFunc, VectorTypeInnerApplyFunc vectorInnerApplyFunc,
		VectorTypeInnerForEachFunc vectorInnerForEachFunc)
		: BasicTypeTraits(std::move(innerCreateFunc), std::move(innerDestroyFunc), std::move(innerApplyFunc))
		, elementType(std::move(elementType)) {

		vectorApplyFunc = [applyFunc = std::move(vectorInnerApplyFunc)](Any& dest, const std::vector<Any>& source) {
			const auto& typeTraits{ getTypeTraits(dest.getTypeId()) };
			if (!std::holds_alternative<VectorTypeTraits>(typeTraits)) {
				return;
			}

			const auto& vectorTypeTraits = std::get<VectorTypeTraits>(typeTraits);

			std::vector<void*> voidVector;
			voidVector.reserve(source.size());

			for (const auto& anyElement : source) {
				if (anyElement.getTypeId() != vectorTypeTraits.elementType) {
					return;
				}

				voidVector.push_back(anyElement.getInstance());
			}

			applyFunc(dest.getInstance(), voidVector);
		};

		vectorForEachFunc = [elementType, forEachFunc = std::move(vectorInnerForEachFunc)](
								const Any& anyVector, std::function<void(const Any&)> visitor) {
			forEachFunc(anyVector.getInstance(), [elementType, visitor = std::move(visitor)](const void* instance) {
				visitor(Any{ elementType, instance });
			});
		};
	}
} // namespace Core
