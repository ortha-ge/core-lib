module;

#include <functional>
#include <memory>
#include <string>

export module Core.NodeHandle;

import Core.Node;

export namespace Core {

	class NodeHandle final {
	public:
		static NodeHandle create(std::string);

		template<class T, typename... Args>
		static NodeHandle create(std::string name, Args&&... args) {
			return NodeHandle(std::make_shared<T>(std::move(name), std::forward<Args>(args)...));
		}

		const Node::Ptr& getNode() const;
		Node* operator->();
		const Node* operator->() const;

	private:
		NodeHandle(Node::Ptr node);

		Node::Ptr mNode;
	};

	Node::Ptr getRootNode(const NodeHandle&);
	void visitParentHeirarchy(const NodeHandle&, const std::function<void(const Node::Ptr&, const Node::Ptr&)>&);

} // namespace Core
