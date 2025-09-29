export module Core.ProcessErrorLoggingSystem;

import Core.ProcessError;
import Core.Scheduler;
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
