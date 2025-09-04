module;

#include <cstddef>

module Core.TypeInfoImpl;

namespace Core {
	TypeInfoImpl::TypeInfoImpl(size_t size)
		: TypeInfo(size) {
	}

	TypeTraits& TypeInfoImpl::getTypeTraits() {
		return mTypeTraits;
	}

	const TypeTraits& TypeInfoImpl::getTypeTraits() const {
		return mTypeTraits;
	}
}