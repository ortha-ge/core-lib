module;

#include <GLFW/glfw3.h>

export module Ortha.Core.GLFWWindow;

export namespace Core {

	struct GLFWWindow {
		::GLFWwindow* window{};
	};

} // namespace Core
