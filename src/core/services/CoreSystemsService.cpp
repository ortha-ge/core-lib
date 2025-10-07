module Ortha.Core.SystemsService;

import Ortha.Core.EntityVisitorSystemService;
import Ortha.Core.FileLoadSystemService;
import Ortha.Core.GLFWSystemService;
import Ortha.Core.GlobalSpatialUpdateSystemService;
import Ortha.Core.LogReportingSystemService;
import Ortha.Core.NodeParentGlobalSpatialUpdateSystemService;
import Ortha.Core.ProcessErrorLoggingSystemService;
import Ortha.Core.ProcessErrorRetrySystemService;
import Ortha.Core.Reflection;
import Ortha.RTTI.ReflectionContext;
import Ortha.RTTI.ReflectionContextStack;
import Ortha.Core.ResourceLoadSystemService;
import Ortha.Core.TypeLoaderSystemService;
import kangaru;

namespace Ortha::Core {

	void initCoreSubSystems(CoreSystems&, kgr::container& container) {
		reflectAll(static_cast<Ortha::RTTI::ReflectionContext&>(Ortha::RTTI::getCurrentReflectionContext()));

		// Platform
		container.emplace<GLFWSystemService>();

		// Visitor
		container.emplace<EntityVisitorSystemService>();

		// GlobalSpatial
		container.emplace<NodeParentGlobalSpatialUpdateSystemService>();
		container.emplace<GlobalSpatialUpdateSystemService>();

		// Resource
		container.emplace<FileLoadSystemService>();
		container.emplace<ResourceLoadSystemService>();
		container.emplace<TypeLoaderSystemService>();

		// ProcessError
		container.emplace<ProcessErrorRetrySystemService>();
		container.emplace<ProcessErrorLoggingSystemService>();

		// Log
		container.emplace<LogReportingSystemService>();
	}

} // namespace Ortha::Core
