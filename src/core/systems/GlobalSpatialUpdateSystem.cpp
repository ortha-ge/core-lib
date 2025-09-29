module;

#include <utility>

#include <tracy/Tracy.hpp>

module Core.GlobalSpatialUpdateSystem;

import Core.GlobalSpatial;
import Core.Spatial;
import glm;

namespace Core {

	GlobalSpatialUpdateSystem::GlobalSpatialUpdateSystem(entt::registry& registry, Scheduler& scheduler)
			: mRegistry(registry)
			, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	GlobalSpatialUpdateSystem::~GlobalSpatialUpdateSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void GlobalSpatialUpdateSystem::tickSystem(entt::registry& registry) {
		ZoneScopedN("GlobalSpatialUpdateSystem::tickSystem");

		registry.view<ParentGlobalSpatial, Spatial>()
			.each([&registry](const entt::entity entity, const ParentGlobalSpatial& parentGlobalSpatial, const Spatial& spatial) {
				auto& globalSpatial = registry.get_or_emplace<GlobalSpatial>(entity);
				globalSpatial.position = parentGlobalSpatial.position + spatial.position;
				globalSpatial.scale = parentGlobalSpatial.scale * spatial.scale;
				globalSpatial.rotation = parentGlobalSpatial.rotation + spatial.rotation;
			});

		registry.view<Spatial>(entt::exclude<ParentGlobalSpatial>)
			.each([&registry](const entt::entity entity, const Spatial& spatial) {
				auto& globalSpatial = registry.get_or_emplace<GlobalSpatial>(entity);
				globalSpatial.position = spatial.position;
				globalSpatial.scale = spatial.scale;
				globalSpatial.rotation = spatial.rotation;
			});
	}

} // namespace Core
