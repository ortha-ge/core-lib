module;

#include <functional>
#include <vector>

#include <entt/entt.hpp>

export module Core.EntityVisitor;

export namespace Core {

	using EntityVisitorAction = std::function<void(entt::entity)>;
	using EntityVisitCompleteAction = std::function<void()>;

	struct EntityVisitor {
		std::vector<entt::entity> sourceEntities{};
		std::vector<EntityVisitorAction> visitActions;
		std::vector<EntityVisitCompleteAction> visitCompleteActions;
	};

} // namespace Core
