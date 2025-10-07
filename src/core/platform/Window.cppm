module;

#include <cstdint>
#include <string>

export module Ortha.Core.Window;

export namespace Ortha::Core {

	struct Window {
		std::string title{ "empty" };
		int32_t width{ 640 };
		int32_t height{ 480 };
	};

} // namespace Ortha::Core
