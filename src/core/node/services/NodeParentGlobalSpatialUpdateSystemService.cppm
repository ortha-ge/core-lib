export module Ortha.Core.NodeParentGlobalSpatialUpdateSystemService;

export import Ortha.Core.NodeParentGlobalSpatialUpdateSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	struct NodeParentGlobalSpatialUpdateSystemService
		: kgr::single_service<NodeParentGlobalSpatialUpdateSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
