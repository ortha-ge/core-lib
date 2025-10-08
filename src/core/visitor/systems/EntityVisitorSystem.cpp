module;

#include <memory>
#include <utility>
#include <vector>

module Ortha.Core.EntityVisitorSystem;

import Ortha.Core.EntityVisitor;
import Ortha.Core.EnTTNode;
import Ortha.Core.Node;
import Ortha.Core.NodeHandle;
import Ortha.RTTI.TypeHandle;
import Ortha.RTTI.TypeId;
import entt;

namespace Ortha::Core::EntityVisitorSystemInternal {

	void tryVisitAsEnTTNode(const std::vector<EntityVisitorAction>& actions, const Node::Ptr& node) {
		using namespace RTTI;
		if (node->getTypeId() != TypeId::get<EnTTNode>()) {
			return;
		}

		auto entityNode = std::static_pointer_cast<EnTTNode>(node);
		const entt::entity entity = entityNode->getEntity();
		for (auto&& action : actions) {
			action(entity);
		}
	}

	void visitNodeAndChildren(const std::vector<EntityVisitorAction>& actions, const Node::Ptr& node, const bool depthFirst = false) {
		if (!node) {
			return;
		}

		if (!depthFirst) {
			tryVisitAsEnTTNode(actions, node);
		}

		for (auto&& childNode : node->getChildren()) {
			visitNodeAndChildren(actions, childNode, depthFirst);
		}

		if (depthFirst) {
			tryVisitAsEnTTNode(actions, node);
		}
	}

} // namespace Ortha::Core::EntityVisitorSystemInternal

namespace Ortha::Core {

	EntityVisitorSystem::EntityVisitorSystem(entt::registry& registry, Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		}, 200);
	}

	EntityVisitorSystem::~EntityVisitorSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void EntityVisitorSystem::tickSystem(entt::registry& registry) {
		using namespace EntityVisitorSystemInternal;

		constexpr bool depthFirst = false;

		registry.view<EntityVisitor>()
			.each([&registry](const EntityVisitor& visitor) {
				for (auto&& sourceEntity : visitor.sourceEntities) {
					if (registry.all_of<NodeHandle>(sourceEntity)) {
						const auto& nodeHandle{ registry.get<NodeHandle>(sourceEntity) };
						visitNodeAndChildren(visitor.visitActions, nodeHandle.getNode(), depthFirst);
					} else {
						for (auto&& action : visitor.visitActions) {
							action(sourceEntity);
						}
					}
				}

				for (auto&& completeAction : visitor.visitCompleteActions) {
					completeAction();
				}
			});
	}

} // namespace Ortha::Core
