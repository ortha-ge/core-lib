module;

#include <kangaru/kangaru.hpp>

export module kangaru;

export namespace kgr {

	template<auto m>
	using invoke_method = kgr::method<decltype(m), m>;

	using kgr::autocall;
	using kgr::container;
	using kgr::dependency;
	using kgr::single_service;

} // namespace kgr
