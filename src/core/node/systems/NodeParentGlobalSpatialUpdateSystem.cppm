module;

#include <entt/fwd.hpp>

export module Core.NodeParentGlobalSpatialUpdateSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Core {

	class NodeParentGlobalSpatialUpdateSystem {
	public:

		NodeParentGlobalSpatialUpdateSystem(EnTTRegistry&, Scheduler&);
		~NodeParentGlobalSpatialUpdateSystem();

		void tickSystem(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Core
