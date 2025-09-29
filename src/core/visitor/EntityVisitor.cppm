module;

#include <functional>
#include <vector>

export module Core.EntityVisitor;

import entt;

export namespace Core {

	using EntityVisitorAction = std::function<void(entt::entity)>;
	using EntityVisitCompleteAction = std::function<void()>;

	struct EntityVisitor {
		std::vector<entt::entity> sourceEntities{};
		std::vector<EntityVisitorAction> visitActions;
		std::vector<EntityVisitCompleteAction> visitCompleteActions;
	};

} // namespace Core
