export module Ortha.Core.Reflection.EnTTNode;

export import Ortha.Core.EnTTNode;

import Ortha.Core.Node;
import Ortha.Core.Reflect;

namespace Ortha::Core {

	template <>
	void reflect<EnTTNode>(RTTI::ReflectionContext& reflectionContext) {
		reflectionContext.addClass<EnTTNode>("EnTTNode")
			.base<Node>()
			;
	}

}
