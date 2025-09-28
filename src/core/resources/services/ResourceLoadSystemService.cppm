export module Core.ResourceLoadSystemService;

export import Core.ResourceLoadSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	struct ResourceLoadSystemService
		: kgr::single_service<ResourceLoadSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
