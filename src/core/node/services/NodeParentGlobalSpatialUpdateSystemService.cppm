export module Core.NodeParentGlobalSpatialUpdateSystemService;

export import Core.NodeParentGlobalSpatialUpdateSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	struct NodeParentGlobalSpatialUpdateSystemService
		: kgr::single_service<NodeParentGlobalSpatialUpdateSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
