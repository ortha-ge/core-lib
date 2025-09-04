module;

#include <variant>

export module Core.TypeTraits;

import Core.TypeId;
import Core.BasicTypeTraits;
import Core.OptionalTypeTraits;
import Core.VectorTypeTraits;
import Core.MapTypeTraits;

export namespace Core {

	struct VoidTypeTraits {};

	using TypeTraits = std::variant<VoidTypeTraits, BasicTypeTraits, OptionalTypeTraits, VectorTypeTraits, MapTypeTraits>;

	const TypeTraits& getTypeTraits(const TypeId&);
}
