module;

#include <variant>

export module Core.TypeTraits;

import Core.TypeId;
import Core.BasicTypeTraits;
import Core.MapTypeTraits;
import Core.OptionalTypeTraits;
import Core.SharedPtrTypeTraits;
import Core.VariantTypeTraits;
import Core.VectorTypeTraits;

export namespace Core {

	struct VoidTypeTraits {};

	using TypeTraits = std::variant<
		VoidTypeTraits, BasicTypeTraits, MapTypeTraits, OptionalTypeTraits, SharedPtrTypeTraits, VariantTypeTraits, VectorTypeTraits>;

	const TypeTraits& getTypeTraits(const TypeId&);
} // namespace Core
