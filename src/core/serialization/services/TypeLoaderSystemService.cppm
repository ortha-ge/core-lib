export module Ortha.Core.TypeLoaderSystemService;

export import Ortha.Core.TypeLoaderSystem;

import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import kangaru;

export namespace Ortha::Core {

	struct TypeLoaderSystemService
		: kgr::single_service<TypeLoaderSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Ortha::Core
