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
import Core.VectorTypeTraits;

namespace Core {

	std::unique_ptr<TypeInfo> createVoidTypeInfo() {
		auto typeInfo = std::make_unique<TypeInfoImpl>(0zu);
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createBasicTypeInfo(
		size_t size, std::function<void*()> createFunc, std::function<void(void*)> destroyFunc,
		std::function<void(void*, const void*)> applyFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			BasicTypeTraits{ TypeId(*typeInfo), std::move(createFunc), std::move(destroyFunc), std::move(applyFunc) };
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createOptionalTypeInfo(
		size_t size, TypeId valueType, std::function<void(void*, const void*)> applyFunc,
		std::function<void*(void*)> getFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			OptionalTypeTraits{ std::move(valueType), std::move(applyFunc), std::move(getFunc) };
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createVectorTypeInfo(
		size_t size, TypeId valueType, std::function<void(void*, const std::vector<void*>&)> applyFunc,
		std::function<void(void*, std::function<void(const void*)>)> forEachFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			VectorTypeTraits{ std::move(valueType), std::move(applyFunc), std::move(forEachFunc) };
		return typeInfo;
	}

	std::unique_ptr<TypeInfo> createMapTypeInfo(
		size_t size, TypeId keyType, TypeId valueType,
		std::function<void(void*, const std::map<void*, void*>&)> applyFunc,
		std::function<void(void*, std::function<void(const void*, const void*)>)> forEachFunc) {
		auto typeInfo = std::make_unique<TypeInfoImpl>(size);
		typeInfo->getTypeTraits() =
			MapTypeTraits{ std::move(keyType), std::move(valueType), std::move(applyFunc), std::move(forEachFunc) };
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

} // namespace Core

size_t std::hash<Core::TypeId>::operator()(const Core::TypeId& typeId) const noexcept {
	return reinterpret_cast<size_t>(&typeId.getTypeInfo());
}
