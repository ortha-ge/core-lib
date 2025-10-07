export module Glm.Reflection.Vec3;

export import glm;

import Ortha.Core.Reflect;

namespace Ortha::Core {

	template<>
	void reflect<glm::vec3>(RTTI::ReflectionContext& reflectionContext) {
		reflectionContext.addClass<glm::vec3>("glm::vec3")
			.field<&glm::vec3::x>("x")
			.field<&glm::vec3::y>("y")
			.field<&glm::vec3::z>("z")
			.field<&glm::vec3::r>("r")
			.field<&glm::vec3::g>("g")
			.field<&glm::vec3::b>("b");
	}

}