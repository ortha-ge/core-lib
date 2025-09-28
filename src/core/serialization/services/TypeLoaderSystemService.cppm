export module Core.TypeLoaderSystemService;

export import Core.TypeLoaderSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	struct TypeLoaderSystemService
		: kgr::single_service<TypeLoaderSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
