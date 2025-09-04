module;

#include <functional>
#include <variant>

module Core.VectorTypeTraits;

import Core.Any;
import Core.TypeId;
import Core.TypeTraits;

namespace Core {
	VectorTypeTraits::VectorTypeTraits(TypeId elementType,
				   std::function<void(void*, const std::vector<void*>&)> applyFunc,
				   std::function<void(void*, std::function<void(const void*)>)> forEachFunc)
				: elementType(std::move(elementType)) {

		this->applyFunc = [applyFunc = std::move(applyFunc)](Any& dest, const std::vector<Any>& source) {
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
		this->forEachFunc = [elementType, forEachFunc = std::move(forEachFunc)](const Any& anyVector, std::function<void(const Any&)> visitor) {
			forEachFunc(anyVector.getInstance(), [elementType, visitor = std::move(visitor)](const void* instance) {
				visitor(Any{ elementType, instance });
			});
		};
	}
}