module;

#include <cstdint>
#include <string>

module Ortha.Core.Reflection;

import Ortha.Core.Reflect;
import Ortha.RTTI.TypeInfo;

namespace Ortha::Core {
	void reflectAll(Ortha::RTTI::ReflectionContext& reflectionContext) {
		using namespace RTTI;

		TypeInfo::getFactory<std::string>().addAlias("std::string");
		TypeInfo::getFactory<int32_t>().addAlias("int32");
		TypeInfo::getFactory<uint8_t>().addAlias("uint8");
		TypeInfo::getFactory<double>().addAlias("double");
		TypeInfo::getFactory<float>().addAlias("float");
		TypeInfo::getFactory<bool>().addAlias("bool");

		reflect<glm::vec2>(reflectionContext);
		reflect<glm::ivec2>(reflectionContext);
		reflect<glm::vec3>(reflectionContext);
		reflect<glm::vec4>(reflectionContext);
		reflect<GlobalSpatial>(reflectionContext);
		reflect<Node>(reflectionContext);
		reflect<Spatial>(reflectionContext);
	}
}