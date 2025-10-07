
export module Ortha.Core.SystemsService;

export import Ortha.Core.Systems;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	void initCoreSubSystems(CoreSystems&, kgr::container&);

	struct CoreSystemsService
		: kgr::single_service<CoreSystems>
		, kgr::autocall<kgr::invoke_method<initCoreSubSystems>>{};

} // namespace Core
