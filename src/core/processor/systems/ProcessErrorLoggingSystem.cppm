module;

#include <entt/fwd.hpp>

export module Core.ProcessErrorLoggingSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Core {

	class ProcessErrorLoggingSystem {
	public:

		ProcessErrorLoggingSystem(EnTTRegistry&, Scheduler&);
		~ProcessErrorLoggingSystem();

		void tickSystem(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
