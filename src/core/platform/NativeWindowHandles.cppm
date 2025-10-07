export module Ortha.Core.NativeWindowHandles;

export namespace Ortha::Core {

	struct NativeWindowHandles {
		void* windowHandle{ nullptr };
		void* displayHandle{ nullptr };
	};

} // namespace Ortha::Core
