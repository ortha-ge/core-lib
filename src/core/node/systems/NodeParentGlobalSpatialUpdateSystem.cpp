module;

#include <memory>
#include <utility>

#include <tracy/Tracy.hpp>

module Ortha.Core.NodeParentGlobalSpatialUpdateSystem;

import Ortha.Core.EnTTNode;
import Ortha.Core.GlobalSpatial;
import Ortha.Core.Node;
import Ortha.Core.NodeHandle;
import Ortha.Core.Spatial;
import Ortha.RTTI.TypeId;
import entt;

namespace Ortha::Core::NodeParentGlobalSpatialUpdateSystemInternal {

	void createParentGlobalSpatial(entt::registry& registry, const entt::entity entity, const NodeHandle& nodeHandle) {
		auto visitor = [&registry](const Node::Ptr& parentNode, const Node::Ptr& node) {
			if (!parentNode) {
				return;
			}

			if (node->getTypeId() != Ortha::RTTI::TypeId::get<EnTTNode>() || parentNode->getTypeId() != Ortha::RTTI::TypeId::get<EnTTNode>()) {
				return;
			}

			const auto enttNode = std::static_pointer_cast<EnTTNode>(node);
			const auto parentEnttNode = std::static_pointer_cast<EnTTNode>(parentNode);

			const entt::entity nodeEntity = enttNode->getEntity();
			const entt::entity parentEntity = parentEnttNode->getEntity();

			if (!registry.all_of<Spatial>(parentEntity)) {
				return;
			}

			ParentGlobalSpatial parentGlobalSpatial;
			if (registry.all_of<ParentGlobalSpatial>(parentEntity)) {
				parentGlobalSpatial = registry.get<ParentGlobalSpatial>(parentEntity);
			}

			const auto& parentSpatial = registry.get<Spatial>(parentEntity);
			parentGlobalSpatial.position += parentSpatial.position;
			parentGlobalSpatial.scale *= parentSpatial.scale;
			parentGlobalSpatial.rotation += parentSpatial.rotation;

			registry.get_or_emplace<ParentGlobalSpatial>(nodeEntity) = std::move(parentGlobalSpatial);
		};
		visitParentHeirarchy(nodeHandle, visitor);
	}
}

namespace Ortha::Core {

	NodeParentGlobalSpatialUpdateSystem::NodeParentGlobalSpatialUpdateSystem(entt::registry& registry, Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	NodeParentGlobalSpatialUpdateSystem::~NodeParentGlobalSpatialUpdateSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void NodeParentGlobalSpatialUpdateSystem::tickSystem(entt::registry& registry) {
		ZoneScopedN("NodeParentGlobalSpatialUpdateSystem::tickSystem");
		using namespace NodeParentGlobalSpatialUpdateSystemInternal;

		registry.view<NodeHandle>()
			.each([&registry](const entt::entity entity, const NodeHandle& nodeHandle) {
				createParentGlobalSpatial(registry, entity, nodeHandle);
			});
	}

} // namespace Ortha::Core
