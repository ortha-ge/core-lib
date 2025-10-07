module;

#include <utility>

#include <tracy/Tracy.hpp>

module Ortha.Core.TypeLoaderSystem;

import Ortha.Core.FileLoadRequest;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.RawDataResource;
import Ortha.Core.TypeLoader;
import entt;

namespace Ortha::Core {

	TypeLoaderSystem::TypeLoaderSystem(entt::registry& registry, Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() { tickSystem(mRegistry); });
	}

	TypeLoaderSystem::~TypeLoaderSystem() { mScheduler.unschedule(std::move(mTickHandle)); }

	void TypeLoaderSystem::tickSystem(entt::registry& registry) {
		ZoneScopedN("TypeLoaderSystem::tickSystem");
		auto jsonLoaderView =
			registry.view<Core::RawDataResource, Core::TypeLoader>(entt::exclude<Core::FileLoadRequest>);
		jsonLoaderView.each(
			[&](entt::entity entity, const Core::RawDataResource& rawDataResource, const Core::TypeLoader& typeLoader) {
				std::string_view rawDataView{ reinterpret_cast<const char*>(rawDataResource.rawData.data()),
											  static_cast<size_t>(rawDataResource.size) };

				typeLoader.adapter->load(registry, entity, rawDataView);

				registry.remove<Core::TypeLoader>(entity);
			});
	}

} // namespace Ortha::Core
