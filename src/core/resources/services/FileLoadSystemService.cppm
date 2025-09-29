export module Core.FileLoadSystemService;

export import Core.FileLoadSystem;

import Core.EnTTRegistryService;
import Core.SchedulerService;
import kangaru;

export namespace Core {

	struct FileLoadSystemService
		: kgr::single_service<FileLoadSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
