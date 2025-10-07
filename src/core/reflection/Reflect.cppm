export module Ortha.Core.Reflect;

export import Ortha.RTTI.ReflectionContext;

export namespace Ortha::Core {

	template<typename T>
	void reflect(RTTI::ReflectionContext&) {
		static_assert(false, "Undefined reflect call for type");
	}

} // namespace Ortha::Core
