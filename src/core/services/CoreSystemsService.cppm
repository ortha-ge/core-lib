
export module Core.SystemsService;

export import Core.Systems;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	void initCoreSubSystems(CoreSystems&, kgr::container&);

	struct CoreSystemsService
		: kgr::single_service<CoreSystems>
		, kgr::autocall<kgr::invoke_method<initCoreSubSystems>>{};

} // namespace Core
