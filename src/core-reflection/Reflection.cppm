export module Ortha.Core.Reflection;

export import Ortha.Core.Reflection.GlobalSpatial;
export import Ortha.Core.Reflection.Node;
export import Ortha.Core.Reflection.Spatial;
export import Glm.Reflection.Vec2;
export import Glm.Reflection.Vec3;
export import Glm.Reflection.Vec4;

import Ortha.RTTI.ReflectionContext;

export namespace Ortha::Core {

	void reflectAll(Ortha::RTTI::ReflectionContext&);

}