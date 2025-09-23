module;

#include <functional>

#include <entt/entt.hpp>

export module Core.EnTTClassAnnotation;

export namespace Core {

	struct EnTTClassAnnotation {
		std::function<void*(entt::registry&, const entt::entity)> getComponent;
	};

	template <typename T>
	EnTTClassAnnotation createEnTTClassAnnotation() {
		EnTTClassAnnotation annotation{
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
