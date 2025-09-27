
export module Core.NodeParentGlobalSpatialUpdateSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.NodeParentGlobalSpatialUpdateSystem;

export namespace Core {

	struct NodeParentGlobalSpatialUpdateSystemService
		: kgr::single_service<NodeParentGlobalSpatialUpdateSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
