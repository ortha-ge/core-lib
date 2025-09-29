export module Core.GlobalSpatialUpdateSystemService;

export import Core.GlobalSpatialUpdateSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace Core {

	struct GlobalSpatialUpdateSystemService
		: kgr::single_service<GlobalSpatialUpdateSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
