export module Core.LogReportingSystem;

import Core.EnTTRegistry;
import Core.Scheduler;
import entt;

export namespace Core {

	class LogReportingSystem {
	public:

		LogReportingSystem(EnTTRegistry&, Scheduler&);
		~LogReportingSystem();

		static void tickSystem(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

	};

} // namespace Core
