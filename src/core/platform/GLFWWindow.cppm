module;

#include <GLFW/glfw3.h>

export module Ortha.Core.GLFWWindow;

export namespace Ortha::Core {

	struct GLFWWindow {
		::GLFWwindow* window{};
	};

} // namespace Ortha::Core
