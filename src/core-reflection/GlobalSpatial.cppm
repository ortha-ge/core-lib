export module Ortha.Core.Reflection.GlobalSpatial;

export import Ortha.Core.GlobalSpatial;

import Ortha.Core.Reflect;
import Ortha.Core.Reflection.Spatial;

namespace Ortha::Core {

	template <>
	void reflect<GlobalSpatial>(RTTI::ReflectionContext& reflectionContext) {
		reflectSpatialType<GlobalSpatial>(reflectionContext, "GlobalSpatial");
	}

}