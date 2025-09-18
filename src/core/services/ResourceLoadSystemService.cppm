
export module Core.ResourceLoadSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.ResourceLoadSystem;

export namespace Core {

	struct ResourceLoadSystemService
		: kgr::single_service<ResourceLoadSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
