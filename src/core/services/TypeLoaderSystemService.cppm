
export module Core.TypeLoaderSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.TypeLoaderSystem;

export namespace Core {

	struct TypeLoaderSystemService
		: kgr::single_service<TypeLoaderSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
