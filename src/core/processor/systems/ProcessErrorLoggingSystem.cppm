export module Ortha.Core.ProcessErrorLoggingSystem;

import Ortha.Core.ProcessError;
import Ortha.Core.Scheduler;
import entt;

export namespace Core {

	class ProcessErrorLoggingSystem {
	public:

		ProcessErrorLoggingSystem(entt::registry&, Scheduler&);
		~ProcessErrorLoggingSystem();

		static void tickSystem(entt::registry&);
		static void tickProcessError(entt::registry&, entt::entity, const ProcessError&);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
