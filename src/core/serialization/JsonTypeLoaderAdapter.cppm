module;

#include <string_view>

export module Core.JsonTypeLoaderAdapter;

import Ortha.RTTI.Any;
import Ortha.RTTI.ReflectionContext;
import Ortha.RTTI.ReflectionContextStack;
import Core.Log;
import Core.Reflect;
import Core.TypeLoader;
import entt;

export namespace Core {

	void load(entt::registry&, const Ortha::RTTI::ReflectionContext&, std::string_view, Ortha::RTTI::Any&);
	void load(Log&, const Ortha::RTTI::ReflectionContext&, std::string_view, Ortha::RTTI::Any&);
	Ortha::RTTI::Any load(entt::registry&, const Ortha::RTTI::ReflectionContext&, std::string_view);
	Ortha::RTTI::Any load(Log&, const Ortha::RTTI::ReflectionContext&, std::string_view);

	template<typename T>
	class JsonTypeLoaderAdapter : public TypeLoaderAdapter {
	public:
		T _loadComponent(entt::registry& registry, entt::entity entity, std::string_view jsonInput) {
			auto& reflectionContext{ static_cast<Ortha::RTTI::ReflectionContext&>(Ortha::RTTI::getCurrentReflectionContext()) };
			//Core::reflectIfValidType<T>(reflectionContext);

			T instance{};
			Ortha::RTTI::Any any(instance);
			Core::load(registry, reflectionContext, jsonInput, any);

			//logEntries(registry, entity, reflectionContext.moveLog());

			return instance;
		}

		template<typename Registry>
		void _load(Registry& registry, entt::entity entity, std::string_view jsonInput) {
			registry.template emplace<T>(entity, _loadComponent(registry, entity, jsonInput));
		}

		void load(entt::registry& registry, entt::entity entity, std::string_view jsonInput) override {
			_load(registry, entity, jsonInput);
		}
	};

} // namespace Core
