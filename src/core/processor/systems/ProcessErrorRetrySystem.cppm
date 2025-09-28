export module Core.ProcessErrorRetrySystem;

import Core.EnTTRegistry;
import Core.ProcessError;
import Core.Scheduler;
import entt;

export namespace Core {

	class ProcessErrorRetrySystem {
	public:

		ProcessErrorRetrySystem(EnTTRegistry&, Scheduler&);
		~ProcessErrorRetrySystem();

		void tickProcessError(entt::registry&, entt::entity, ProcessError&);
		void tickSystem(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
