export module Ortha.Core.EnTTRegistryService;

import entt;
import kangaru;

export namespace Core {

	class EnTTRegistryService : public kgr::single_service<entt::registry> {};

} // namespace Core
