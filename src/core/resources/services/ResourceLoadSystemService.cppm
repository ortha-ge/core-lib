export module Ortha.Core.ResourceLoadSystemService;

export import Ortha.Core.ResourceLoadSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	struct ResourceLoadSystemService
		: kgr::single_service<ResourceLoadSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
