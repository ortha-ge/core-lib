module;

#include <memory>
#include <string_view>

#include <entt/fwd.hpp>

export module Core.TypeLoader;

export namespace Core {

	class TypeLoaderAdapter {
	public:
		virtual ~TypeLoaderAdapter() = default;

		virtual void load(entt::registry&, entt::entity, std::string_view) = 0;
	};

	struct TypeLoader {
		std::shared_ptr<TypeLoaderAdapter> adapter;
	};

} // namespace Core
