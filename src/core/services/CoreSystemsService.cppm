
export module Core.SystemsService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
export import Core.Systems;

export namespace Core {

	class CoreSystemsService : public kgr::single_service<CoreSystems, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // Core
