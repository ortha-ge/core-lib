module;

export module Core.EntityVisitorSystem;

import Core.Scheduler;
import entt;

export namespace Core {

	class EntityVisitorSystem {
	public:

		EntityVisitorSystem(entt::registry&, Scheduler&);
		~EntityVisitorSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Core
