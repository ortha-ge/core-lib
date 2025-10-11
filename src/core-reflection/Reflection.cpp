module;

#include <cstdint>
#include <string>

module Ortha.Core.Reflection;

import Ortha.RTTI.Reflect;
import Ortha.RTTI.TypeInfo;

namespace Ortha::Core {
	void reflectAll(RTTI::ReflectionContext& reflectionContext) {
		using namespace RTTI;

		TypeInfo::getFactory<std::string>().addAlias("std::string");
		TypeInfo::getFactory<int32_t>().addAlias("int32");
		TypeInfo::getFactory<uint8_t>().addAlias("uint8");
		TypeInfo::getFactory<double>().addAlias("double");
		TypeInfo::getFactory<float>().addAlias("float");
		TypeInfo::getFactory<bool>().addAlias("bool");

		ensureReflected<EnTTNode>(reflectionContext);
		ensureReflected<GlobalSpatial>(reflectionContext);
		ensureReflected<Node>(reflectionContext);
		ensureReflected<Spatial>(reflectionContext);
	}
}