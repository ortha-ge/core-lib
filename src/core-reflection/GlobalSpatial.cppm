export module Core.Reflection.GlobalSpatial;

export import Core.GlobalSpatial;

import Core.Reflect;
import Core.Reflection.Spatial;

template <>
void Core::reflect<Core::GlobalSpatial>(Ortha::RTTI::ReflectionContext& reflectionContext) {
	reflectSpatialType<GlobalSpatial>(reflectionContext, "GlobalSpatial");
}