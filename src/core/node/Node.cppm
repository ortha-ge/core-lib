module;

#include <functional>
#include <memory>
#include <string>
#include <vector>

export module Core.Node;

import Core.TypeId;

export namespace Core {

	class Node : public std::enable_shared_from_this<Node> {
	public:
		using Ptr = std::shared_ptr<Node>;
		using WeakPtr = std::weak_ptr<Node>;

		Node();
		Node(std::string name);
		virtual ~Node() {}

		virtual void destroy();
		virtual TypeId getTypeId() const;

		void addChild(Ptr child);
		void removeChild(const Ptr& child);

		void setParent(Ptr parentNode);

		const std::string& getName() const;
		Ptr getParentNode() const;
		const std::vector<Ptr>& getChildren() const;

	private:
		std::string mName;
		WeakPtr mParent;
		std::vector<Ptr> mChildren;
	};

	class NodeHandle final {
	public:
		static NodeHandle create(std::string);

		template<class T, typename... Args>
		static NodeHandle create(std::string name, Args&&... args) {
			return NodeHandle(std::make_shared<T>(std::move(name), std::forward<Args>(args)...));
		}

		const Node::Ptr& getNode() const;

	private:
		NodeHandle(Node::Ptr node);

		Node::Ptr mNode;
	};

	Node::Ptr getRootNode(const NodeHandle&);
	void visitParentHeirarchy(const NodeHandle&, const std::function<void(const Node::Ptr&, const Node::Ptr&)>&);

} // namespace Core
