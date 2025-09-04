module;

#include <cstddef>

export module Core.TypeInfoImpl;

import Core.TypeInfo;
import Core.TypeTraits;

export namespace Core {
	class TypeInfoImpl : public TypeInfo {
	public:

		TypeInfoImpl(size_t size);

		TypeTraits& getTypeTraits();
		const TypeTraits& getTypeTraits() const;

	private:

		TypeTraits mTypeTraits;

	};
}