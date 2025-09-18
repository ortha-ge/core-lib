
export module Core.SystemsService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
export import Core.Systems;

export namespace Core {

	void initCoreSubSystems(CoreSystems&, kgr::container&);

	struct CoreSystemsService
		: kgr::single_service<CoreSystems>
		, kgr::autocall<kgr::invoke_method<initCoreSubSystems>>{};

} // namespace Core
