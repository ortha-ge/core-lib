module;

#include <functional>

#include <entt/entt.hpp>

export module Core.EnTTComponentAttribute;

export namespace Core {

	struct EnTTComponentAttribute {
		std::function<void*(entt::registry&, const entt::entity)> getComponent;
	};

	template <typename T>
	EnTTComponentAttribute createEnTTComponentAttribute() {
		EnTTComponentAttribute annotation{
			// TODO: change to returns Any
			[](entt::registry& registry, entt::entity entity) -> void* {
				if (registry.all_of<T>(entity)) {
					return &registry.get<T>(entity);
				}

				return nullptr;
			}
		};
		return annotation;
	}

} // namespace Core
