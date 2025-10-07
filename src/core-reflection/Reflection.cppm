export module Core.Reflection;

export import Core.Reflection.GlobalSpatial;
export import Core.Reflection.Node;
export import Core.Reflection.Spatial;
export import Glm.Reflection.Vec2;
export import Glm.Reflection.Vec3;
export import Glm.Reflection.Vec4;

import Ortha.RTTI.ReflectionContext;

export namespace Core {

	void reflectAll(Ortha::RTTI::ReflectionContext&);

}