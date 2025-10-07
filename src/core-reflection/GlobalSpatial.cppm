export module Ortha.Core.Reflection.GlobalSpatial;

export import Ortha.Core.GlobalSpatial;

import Ortha.Core.Reflect;
import Ortha.Core.Reflection.Spatial;

template <>
void Core::reflect<Core::GlobalSpatial>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	reflectSpatialType<GlobalSpatial>(reflectionContext, "GlobalSpatial");
}