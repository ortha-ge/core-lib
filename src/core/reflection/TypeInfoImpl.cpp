module;

#include <cstddef>
#include <utility>

module Core.TypeInfoImpl;

namespace Core {

	TypeInfoImpl::TypeInfoImpl(size_t size)
		: TypeInfoImpl(size, {}) {
	}

	TypeInfoImpl::TypeInfoImpl(size_t size, TypeTraits typeTraits)
		: TypeInfo(size)
		, mTypeTraits(std::move(typeTraits)) {
	}

	const TypeTraits& TypeInfoImpl::getTypeTraits() const { return mTypeTraits; }
} // namespace Core
