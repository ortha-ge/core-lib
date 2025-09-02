
export module Core.NativeWindowHandles;

export namespace Core {

	struct NativeWindowHandles {
		void* windowHandle{ nullptr };
		void* displayHandle{ nullptr };
	};

} // namespace Core
