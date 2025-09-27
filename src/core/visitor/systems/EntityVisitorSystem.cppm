module;

#include <entt/fwd.hpp>

export module Core.EntityVisitorSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Core {

	class EntityVisitorSystem {
	public:

		EntityVisitorSystem(EnTTRegistry&, Scheduler&);
		~EntityVisitorSystem();

		void tickSystem(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;

	};

} // namespace Core
