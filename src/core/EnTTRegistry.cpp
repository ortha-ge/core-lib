module;

#include <entt/entt.hpp>

module Core.EnTTRegistry;

namespace Core {

	EnTTRegistry::EnTTRegistry()
		: mRegistry { std::make_unique<entt::registry>() } {
	}

	EnTTRegistry::operator entt::registry&() {
		return *mRegistry;
	}

	EnTTRegistry::operator const entt::registry&() const {
		return *mRegistry;
	}

} // Core