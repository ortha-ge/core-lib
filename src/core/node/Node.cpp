module;

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <vector>

module Core.Node;

namespace Core {

	Node::Node() = default;

	Node::Node(std::string name)
		: mName(std::move(name)) {}

	void Node::destroy() {
		for (auto&& child : mChildren) {
			child->destroy();
		}

		mChildren.clear();
	}

	TypeId Node::getTypeId() const {
		return TypeId::get<Node>();
	}

	void Node::addChild(Ptr child) {
		if (!child) {
			return;
		}

		auto parentNode = child->getParentNode();
		if (parentNode == shared_from_this()) {
			return;
		}

		if (parentNode) {
			parentNode->removeChild(child);
		}

		if (std::ranges::find(mChildren, child) != mChildren.end()) {
			return;
		}

		child->setParent(shared_from_this());
		mChildren.push_back(std::move(child));
	}

	void Node::removeChild(const Ptr& child) {
		if (!child) {
			return;
		}

		auto parentNode = child->getParentNode();
		if (parentNode != shared_from_this()) {
			return;
		}

		if (parentNode) {
			child->setParent({});
			;
		}

		auto it = std::ranges::find(mChildren, child);
		if (it != mChildren.end()) {
			mChildren.erase(it);
		}
	}

	void Node::setParent(Ptr parentNode) { mParent = parentNode; }

	const std::string& Node::getName() const { return mName; }

	Node::Ptr Node::getParentNode() const { return mParent.lock(); }

	const std::vector<Node::Ptr>& Node::getChildren() const { return mChildren; }

	NodeHandle NodeHandle::create(std::string name) { return NodeHandle(std::make_shared<Node>(name)); }

	const Node::Ptr& NodeHandle::getNode() const { return mNode; }

	Node* NodeHandle::operator->() {
		return mNode.get();
	}
	const Node* NodeHandle::operator->() const {
		return mNode.get();
	}

	NodeHandle::NodeHandle(Node::Ptr node)
		: mNode(std::move(node)) {}

	Node::Ptr getRootNode(const NodeHandle& node) {
		Node::Ptr rootNode = node.getNode();
		while (rootNode) {
			Node::Ptr parentNode = rootNode->getParentNode();
			if (!parentNode) {
				break;
			}

			rootNode = parentNode;
		}

		return rootNode;
	}

	void _visitParentHeirarchy(Node::Ptr node, const std::function<void(const Node::Ptr&, const Node::Ptr&)>& visitor) {
		if (!node) {
			return;
		}

		Node::Ptr parentNode = node->getParentNode();
		_visitParentHeirarchy(parentNode, visitor);
		visitor(parentNode, node);
	}

	void visitParentHeirarchy(const NodeHandle& node, const std::function<void(const Node::Ptr&, const Node::Ptr&)>& visitor) {
		_visitParentHeirarchy(node.getNode(), visitor);
	}

} // namespace Core
