module Core.Reflection;

namespace Core {
	void reflectAll(ReflectionContext& reflectionContext) {
		reflect<GlobalSpatial>(reflectionContext);
		reflect<Node>(reflectionContext);
		reflect<Spatial>(reflectionContext);
	}
}