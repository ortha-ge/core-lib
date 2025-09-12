module;

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

module Core.Node;

namespace Core {

	Node::Node(std::string name)
		: mName(std::move(name)) {}

	void Node::destroy() {
		for (auto&& child : mChildren) {
			child->destroy();
		}

		mChildren.clear();
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

} // namespace Core
