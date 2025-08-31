module;

#include <entt/fwd.hpp>

export module Core.GLFWSystem;

export namespace Core {
	class GLFWSystem {
	public:

		static void initSystem(entt::registry& registry);
		static void destroySystem(entt::registry& registry);
		static void tickSystem(entt::registry& registry);

		static void tickCreateWindowView(entt::registry& registry);
		static void tickCloseWindowView(entt::registry& registry);

	};
} // Core
