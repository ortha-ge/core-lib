module;

#include <entt/fwd.hpp>

export module Core.FileLoadSystem;

export namespace Core {

	class FileLoadSystem {
	public:
		static void initSystem(entt::registry& registry);
		static void destroySystem(entt::registry& registry);
		static void tickSystem(entt::registry& registry);
	};

} // namespace Core
