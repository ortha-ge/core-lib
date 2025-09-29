export module Core.GlobalSpatialUpdateSystem;

import Core.Scheduler;
import entt;

export namespace Core {

	class GlobalSpatialUpdateSystem {
	public:

		GlobalSpatialUpdateSystem(entt::registry&, Scheduler&);
		~GlobalSpatialUpdateSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Core
