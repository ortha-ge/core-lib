export module Core.NodeParentGlobalSpatialUpdateSystem;

import Core.Scheduler;
import entt;

export namespace Core {

	class NodeParentGlobalSpatialUpdateSystem {
	public:

		NodeParentGlobalSpatialUpdateSystem(entt::registry&, Scheduler&);
		~NodeParentGlobalSpatialUpdateSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Core
