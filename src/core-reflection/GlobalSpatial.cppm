export module Core.Reflection.GlobalSpatial;

export import Core.GlobalSpatial;

import Core.ReflectionContext;
import Core.Reflection.Spatial;

template <>
void Core::reflect<Core::GlobalSpatial>(ReflectionContext& reflectionContext) {
	reflectSpatialType<GlobalSpatial>(reflectionContext, "GlobalSpatial");
}