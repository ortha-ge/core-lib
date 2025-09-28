module;

#include <string>

module Core.EnTTNode;

import Core.ProcessError;
import entt;

namespace Core {

	EnTTNode::EnTTNode() = default;

	EnTTNode::EnTTNode(std::string name, entt::handle entityHandle)
		: Node(std::move(name))
		, mEntityHandle(entityHandle) {}

	EnTTNode::~EnTTNode() = default;

	void EnTTNode::destroy() {
		Node::destroy();

		if (mEntityHandle.valid()) {
			mEntityHandle.destroy();
		}
	}

	TypeId EnTTNode::getTypeId() const {
		return TypeId::get<EnTTNode>();
	}

	entt::entity EnTTNode::getEntity() const {
		return mEntityHandle.entity();
	}

	const entt::entity createEnTTNode(entt::registry& registry, std::string name) {
		const entt::entity entity = registry.create();
		registry.emplace<NodeHandle>(entity, NodeHandle::create<EnTTNode>(std::move(name), entt::handle(registry, entity)));
		return entity;
	}

	const entt::entity createChildEnTTNode(entt::registry& registry, const entt::entity parent, std::string name) {
		const auto* parentNodeHandle = registry.try_get<NodeHandle>(parent);
		if (!parentNodeHandle) {
			addProcessError(registry, parent, "Specified parent entity doesn't have a NodeHandle.");
			return entt::null;
		}

		const entt::entity childEntity = createEnTTNode(registry, std::move(name));
		const auto& childNodeHandle{ registry.get<NodeHandle>(childEntity) };
		parentNodeHandle->getNode()->addChild(childNodeHandle.getNode());
		return childEntity;
	}

	void addChildNode(entt::registry& registry, const entt::entity parent, const entt::entity child) {
		const auto* parentNodeHandle = registry.try_get<NodeHandle>(parent);
		const auto* childNodeHandle = registry.try_get<NodeHandle>(child);
		if (!parentNodeHandle || !childNodeHandle) {
			addProcessError(registry, parent, "Specified parent entity doesn't have a NodeHandle.");
			return;
		}

		parentNodeHandle->getNode()->addChild(childNodeHandle->getNode());
	}

} // namespace Core
