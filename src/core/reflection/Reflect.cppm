export module Core.Reflect;

export import Ortha.RTTI.ReflectionContext;

export namespace Core {

	template<typename T>
	void reflect(Ortha::RTTI::ReflectionContext&) {
		static_assert(false, "Undefined reflect call for type");
	}

} // namespace Core
