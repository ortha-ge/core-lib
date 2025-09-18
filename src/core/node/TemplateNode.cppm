module;

#include <memory>
#include <string>

export module Core.TemplateNode;

import Core.Node;
import Core.TypeId;

export namespace Core {

	template <typename T>
	class TemplateNode : public Node {
	public:

		TemplateNode() = default;

		template <typename... Args>
		TemplateNode(std::string name, Args&&... args)
			: Node(std::move(name))
			, mInstance(std::make_shared<T>(std::forward<Args>(args)...)) {
		}

		TypeId getTypeId() const override {
			return TypeId::get<TemplateNode<T>>();
		}

	private:
		std::shared_ptr<T> mInstance;
	};

} // namespace Core
