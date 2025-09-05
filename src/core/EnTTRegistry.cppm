module;

#include <entt/fwd.hpp>

export module Core.EnTTRegistry;

export namespace Core {

	class EnTTRegistry {
	public:
		EnTTRegistry();

		operator entt::registry&();
		operator const entt::registry&() const;

	private:
		std::unique_ptr<entt::registry> mRegistry{};
	};

} // namespace Core
