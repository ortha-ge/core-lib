module;

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

export module Core.ResourceLoadSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace Core {

	class ResourceLoadSystem {
	public:

		ResourceLoadSystem(EnTTRegistry&, Scheduler&);
		~ResourceLoadSystem();

		void tick(entt::registry&);

	private:

		EnTTRegistry& mRegistry;
		Scheduler& mScheduler;
		TaskHandle mTickHandle{};

		entt::entity mResourceCacheEntity{ entt::null };

	};

} // namespace Core
