module;

#include <memory>
#include <string_view>

export module Ortha.Core.TypeLoader;

import entt;

export namespace Ortha::Core {

	class TypeLoaderAdapter {
	public:
		virtual ~TypeLoaderAdapter() = default;

		virtual void load(entt::registry&, entt::entity, std::string_view) = 0;
	};

	struct TypeLoader {
		std::shared_ptr<TypeLoaderAdapter> adapter;
	};

} // namespace Ortha::Core
