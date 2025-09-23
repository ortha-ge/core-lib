module Core.Reflection;

namespace Core {
	void reflectAll(ReflectionContext& reflectionContext) {
		reflect<GlobalSpatial>(reflectionContext);
		reflect<Spatial>(reflectionContext);
	}
}