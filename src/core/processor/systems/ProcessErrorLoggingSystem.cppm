module;

#include <entt/fwd.hpp>

export module Core.ProcessErrorLoggingSystem;

import Core.EnTTRegistry;
import Core.ProcessError;
import Core.Scheduler;

export namespace Core {

	class ProcessErrorLoggingSystem {
	public:

		ProcessErrorLoggingSystem(EnTTRegistry&, Scheduler&);
		~ProcessErrorLoggingSystem();

		static void tickSystem(entt::registry&);
		static void tickProcessError(entt::registry&, entt::entity, const ProcessError&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
