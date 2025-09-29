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

			std::vector<TypeInstance> voidVector;
			voidVector.reserve(source.size());

			for (const auto& anyElement : source) {
				if (anyElement.getTypeId() != vectorTypeTraits.elementType) {
					return;
				}

				voidVector.push_back(anyElement.getTypeInstance());
			}

			applyFunc(dest.getTypeInstance(), voidVector);
		};

		vectorForEachFunc = [elementType, forEachFunc = std::move(vectorInnerForEachFunc)](
								const Any& anyVector, std::function<void(const Any&)> visitor) {
			forEachFunc(anyVector.getTypeInstance(), [elementType, visitor = std::move(visitor)](const TypeInstance& instance) {
				visitor(Any{ instance });
			});
		};
	}
} // namespace Core
