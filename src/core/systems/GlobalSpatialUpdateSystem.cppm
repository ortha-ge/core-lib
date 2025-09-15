module;

#include <entt/fwd.hpp>

export module Core.GlobalSpatialUpdateSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Core {

	class GlobalSpatialUpdateSystem {
	public:

		GlobalSpatialUpdateSystem(EnTTRegistry&, Scheduler&);
		~GlobalSpatialUpdateSystem();

		void tickSystem(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Core
