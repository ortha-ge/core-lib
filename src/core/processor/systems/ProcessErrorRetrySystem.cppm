export module Core.ProcessErrorRetrySystem;

import Core.ProcessError;
import Core.Scheduler;
import entt;

export namespace Core {

	class ProcessErrorRetrySystem {
	public:

		ProcessErrorRetrySystem(entt::registry&, Scheduler&);
		~ProcessErrorRetrySystem();

		void tickProcessError(entt::registry&, entt::entity, ProcessError&);
		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
