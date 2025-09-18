module;

#include <entt/fwd.hpp>

export module Core.GLFWSystem;

import Core.EnTTRegistry;
import Core.Scheduler;
import Core.Window;

export namespace Core {
	class GLFWSystem {
	public:

		GLFWSystem(EnTTRegistry&, Scheduler&);
		~GLFWSystem();

		static void tickSystem(entt::registry&);

		static void destroyWindows(entt::registry&);

		static void tickCreateWindowView(entt::registry& registry);
		static void tickCloseWindowView(entt::registry& registry);

		static void tryCreateGLFWWindow(entt::registry&, entt::entity, const Window&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};
} // namespace Core
