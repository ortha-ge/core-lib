module Core.TypeTraits;

import Core.TypeInfoImpl;

namespace Core {
	const TypeTraits& getTypeTraits(const TypeId& typeId) {
		return static_cast<const TypeInfoImpl&>(typeId.getTypeInfo()).getTypeTraits();
	}
}