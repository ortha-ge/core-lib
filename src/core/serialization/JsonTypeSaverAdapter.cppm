module;

#include <string>
#include <string_view>

export module Core.JsonTypeSaverAdapter;

import Core.Any;
import Core.Log;
import Core.TypeSaver;
import entt;

export namespace Core {

	std::string save(entt::registry&, const Any& anyValue);
	std::string save(Log&, const Any& anyValue);

	template<typename T>
	class JsonTypeSaverAdapter : public TypeSaverAdapter {
	public:
		std::string _save(entt::registry& registry, const T& instance) {
			Any any(instance);
			return Core::save(registry, any);
		}

		template<typename Registry>
		void _save(std::string_view jsonInput, Registry& registry, entt::entity entity) {
			// registry.template emplace<T>(entity, _save(jsonInput));
		}

		void save(std::string_view jsonInput, entt::registry& registry, entt::entity entity) override {
			//_save(jsonInput, registry, entity);
		}
	};

} // namespace Core
