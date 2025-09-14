module;

#include <cstddef>

export module Core.TypeInfoImpl;

import Core.TypeInfo;
import Core.TypeTraits;

export namespace Core {
	class TypeInfoImpl : public TypeInfo {
	public:
		TypeInfoImpl(size_t size);
		TypeInfoImpl(size_t size, TypeTraits typeTraits);

		const TypeTraits& getTypeTraits() const;

	private:
		TypeTraits mTypeTraits;
	};
} // namespace Core
