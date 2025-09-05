module;

#include <utility>

#include <entt/entt.hpp>

module Core.TypeLoaderSystem;

import Core.FileLoadRequest;
import Core.JsonTypeLoaderAdapter;
import Core.RawDataResource;
import Core.TypeLoader;

namespace Core {

	TypeLoaderSystem::TypeLoaderSystem(EnTTRegistry& registry, Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	TypeLoaderSystem::~TypeLoaderSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void TypeLoaderSystem::tickSystem(entt::registry& registry) {
		auto jsonLoaderView =
			registry.view<Core::RawDataResource, Core::TypeLoader>(entt::exclude<Core::FileLoadRequest>);
		jsonLoaderView.each(
			[&](entt::entity entity, const Core::RawDataResource& rawDataResource, const Core::TypeLoader& typeLoader) {
				std::string_view rawDataView{ reinterpret_cast<const char*>(rawDataResource.rawData.data()),
											  static_cast<size_t>(rawDataResource.size) };

				typeLoader.adapter->load(rawDataView, registry, entity);

				registry.remove<Core::TypeLoader>(entity);
			});
	}

} // namespace Core
