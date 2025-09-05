module;

#include <cstdint>
#include <string>

export module Core.Window;

export namespace Core {

	struct Window {
		std::string title{ "empty" };
		int32_t width{ 640 };
		int32_t height{ 480 };
	};

} // namespace Core
