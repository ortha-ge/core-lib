
export module Core.EnTTRegistryService;

export import Core.EnTTRegistry;
import kangaru;

export namespace Core {

	class EnTTRegistryService : public kgr::single_service<EnTTRegistry> {};

} // namespace Core
