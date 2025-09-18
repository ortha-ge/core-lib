
export module Core.FileLoadSystemService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export import Core.FileLoadSystem;

export namespace Core {

	struct FileLoadSystemService
		: kgr::single_service<FileLoadSystem, kgr::dependency<EnTTRegistryService, SchedulerService>>{};

} // namespace Core
