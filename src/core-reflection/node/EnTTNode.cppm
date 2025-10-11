export module Ortha.Core.Reflection.EnTTNode;

export import Ortha.Core.EnTTNode;

import Ortha.Core.Reflection.Node;
import Ortha.RTTI.ReflectionContext;

namespace Ortha::RTTI {

	template <>
	void reflect<Core::EnTTNode>(ReflectionContext& reflectionContext) {
		using namespace Core;
		reflectionContext.addClass<EnTTNode>("EnTTNode")
			.base<Node>();
	}

}
