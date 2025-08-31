module;

#include <GLFW/glfw3.h>

export module Core.WindowInternal;

export namespace Core {

	struct WindowInternal {
		GLFWwindow* window{};
	};

} // Core
