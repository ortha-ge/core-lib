
export module Ortha.Core.Systems;

export namespace Ortha::Core {

	class CoreSystems {
	public:
		CoreSystems();
		~CoreSystems();

		CoreSystems(CoreSystems&&) = delete;
		CoreSystems& operator=(CoreSystems&&) noexcept = delete;

		CoreSystems(const CoreSystems&) = delete;
		CoreSystems& operator=(const CoreSystems&) = delete;

	};

} // namespace Ortha::Core
