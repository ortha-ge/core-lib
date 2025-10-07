export module Ortha.Core.EnTTRegistryService;

import entt;
import kangaru;

export namespace Ortha::Core {

	class EnTTRegistryService : public kgr::single_service<entt::registry> {};

} // namespace Ortha::Core
