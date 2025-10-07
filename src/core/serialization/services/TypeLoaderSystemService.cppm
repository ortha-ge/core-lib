export module Ortha.Core.TypeLoaderSystemService;

export import Ortha.Core.TypeLoaderSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Core {

	struct TypeLoaderSystemService
		: kgr::single_service<TypeLoaderSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
