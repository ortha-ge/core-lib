
export module Core.GlobalSpatialUpdateSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.GlobalSpatialUpdateSystem;

export namespace Core {

	struct GlobalSpatialUpdateSystemService
		: kgr::single_service<GlobalSpatialUpdateSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
