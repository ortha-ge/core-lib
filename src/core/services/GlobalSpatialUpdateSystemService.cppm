export module Ortha.Core.GlobalSpatialUpdateSystemService;

export import Ortha.Core.GlobalSpatialUpdateSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::Core {

	struct GlobalSpatialUpdateSystemService
		: kgr::single_service<GlobalSpatialUpdateSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Ortha::Core
