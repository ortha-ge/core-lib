module;

#include <functional>
#include <memory>
#include <string>

export module Ortha.Core.NodeHandle;

import Ortha.Core.Node;

export namespace Core {

	class NodeHandle final {
	public:

		NodeHandle(Node::Ptr node);

		static NodeHandle create(std::string);

		template<class T, typename... Args>
		static NodeHandle create(std::string name, Args&&... args) {
			return NodeHandle(std::make_shared<T>(std::move(name), std::forward<Args>(args)...));
		}

		const Node::Ptr& getNode() const;
		Node* operator->();
		const Node* operator->() const;

	private:

		Node::Ptr mNode;
	};

	Node::Ptr getRootNode(const NodeHandle&);
	void visitParentHeirarchy(const NodeHandle&, const std::function<void(const Node::Ptr&, const Node::Ptr&)>&);

} // namespace Core
