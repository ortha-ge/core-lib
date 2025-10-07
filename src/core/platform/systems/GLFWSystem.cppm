export module Ortha.Core.GLFWSystem;

import Ortha.Core.Scheduler;
import Ortha.Core.Window;
import entt;

export namespace Ortha::Core {
	class GLFWSystem {
	public:

		GLFWSystem(entt::registry&, Scheduler&);
		~GLFWSystem();

		static void tickSystem(entt::registry&);

		static void destroyWindows(entt::registry&);

		static void tickCreateWindowView(entt::registry& registry);
		static void tickCloseWindowView(entt::registry& registry);

		static void tryCreateGLFWWindow(entt::registry&, entt::entity, const Window&);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};
} // namespace Ortha::Core
