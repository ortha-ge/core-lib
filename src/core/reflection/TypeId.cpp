module;

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <vector>

module Core.TypeId;

import Core.TypeInfoImpl;
import Core.BasicTypeTraits;
import Core.MapTypeTraits;
import Core.OptionalTypeTraits;
import Core.VariantTypeTraits;
import Core.VectorTypeTraits;

namespace Core {

	std::unique_ptr<TypeInfo> createVoidTypeInfo() {
		auto typeInfo = std::make_unique<TypeInfoImpl>(0zu);
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createBasicTypeInfo(
		size_t size, BasicTypeInnerCreateFunc createFunc, BasicTypeInnerDestroyFunc destroyFunc,
		BasicTypeInnerApplyFunc applyFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			BasicTypeTraits{ TypeId(*typeInfo), std::move(createFunc), std::move(destroyFunc), std::move(applyFunc) };
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createOptionalTypeInfo(
		size_t size, TypeId valueType, BasicTypeInnerCreateFunc createFunc, BasicTypeInnerDestroyFunc destroyFunc,
		BasicTypeInnerApplyFunc applyFunc, OptionalTypeInnerApplyFunc optionalApplyFunc,
		OptionalTypeInnerGetFunc optionalGetFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			OptionalTypeTraits{ TypeId(*typeInfo), std::move(valueType), std::move(createFunc),		std::move(destroyFunc),
								std::move(applyFunc), std::move(optionalApplyFunc), std::move(optionalGetFunc) };
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createVectorTypeInfo(
		size_t size, TypeId valueType, BasicTypeInnerCreateFunc createFunc, BasicTypeInnerDestroyFunc destroyFunc,
		BasicTypeInnerApplyFunc applyFunc, VectorTypeInnerApplyFunc vectorApplyFunc,
		VectorTypeInnerForEachFunc vectorForEachFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			VectorTypeTraits{ TypeId(*typeInfo),		   std::move(valueType), std::move(createFunc),
							  std::move(destroyFunc),	   std::move(applyFunc), std::move(vectorApplyFunc),
							  std::move(vectorForEachFunc) };
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createMapTypeInfo(
		size_t size, TypeId keyType, TypeId valueType, BasicTypeInnerCreateFunc createFunc,
		BasicTypeInnerDestroyFunc destroyFunc, BasicTypeInnerApplyFunc applyFunc, MapTypeInnerApplyFunc mapApplyFunc,
		MapTypeInnerForEachFunc mapForEachFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			MapTypeTraits{ TypeId(*typeInfo), std::move(keyType),		 std::move(valueType), std::move(createFunc),
						   std::move(destroyFunc),	 std::move(applyFunc), std::move(mapApplyFunc),
						   std::move(mapForEachFunc) };
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createVariantTypeInfo(size_t size,
		std::vector<TypeId> types, BasicTypeInnerCreateFunc createFunc, BasicTypeInnerDestroyFunc destroyFunc,
		BasicTypeInnerApplyFunc applyFunc, VariantTypeInnerApplyFunc variantApplyFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() = VariantTypeTraits{ TypeId(*typeInfo), std::move(types), std::move(createFunc), std::move(destroyFunc),
													   std::move(applyFunc), std::move(variantApplyFunc) };
		return typeInfo;
	}

	TypeId::TypeId()
		: mTypeInfo(TypeId::get<void>().mTypeInfo) {}

	TypeId::TypeId(const TypeInfo& typeInfo)
		: mTypeInfo(&typeInfo) {}

	TypeId::TypeId(const TypeId&) = default;
	TypeId::TypeId(TypeId&&) noexcept = default;
	TypeId::~TypeId() = default;

	TypeId& TypeId::operator=(const TypeId&) = default;
	TypeId& TypeId::operator=(TypeId&&) noexcept = default;

	bool TypeId::operator==(const TypeId&) const = default;

	bool TypeId::operator==(const TypeInfo& other) const { return mTypeInfo == &other; }

	bool TypeId::operator==(const TypeInfo* other) const { return mTypeInfo == other; }

	const TypeInfo& TypeId::getTypeInfo() const {
		if (mTypeInfo) {
			return *mTypeInfo;
		}

		static const TypeInfoImpl invalidTypeInfo{ 0zu };
		return invalidTypeInfo;
	}

	std::size_t TypeIdHasher::operator()(const TypeId& typeId) const noexcept {
		return reinterpret_cast<size_t>(&typeId.getTypeInfo());
	}

} // namespace Core
