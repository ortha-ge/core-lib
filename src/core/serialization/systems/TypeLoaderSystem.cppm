export module Core.TypeLoaderSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

import entt;

export namespace Core {

	class TypeLoaderSystem {
	public:
		TypeLoaderSystem(EnTTRegistry&, Scheduler&);
		~TypeLoaderSystem();

		void tickSystem(entt::registry&);

	private:
		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle;
	};

} // namespace Core
