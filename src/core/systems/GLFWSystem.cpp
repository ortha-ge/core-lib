
module;

#include <chrono>
#include <entt/entt.hpp>
#include <GLFW/glfw3.h>

module Core.GLFWSystem;

import Core.QuitAppRequest;
import Core.Window;
import Core.WindowInternal;

namespace Core {

	void GLFWSystem::initSystem(entt::registry& registry) {
		if (glfwInit() != GLFW_TRUE) {
			return;
		}
		
		int monitorCount{ 0 };
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		if (monitorCount <= 0) {
			return;
		}
	}
	
	void GLFWSystem::destroySystem(entt::registry& registry) {
		auto internalWindowView = registry.view<WindowInternal>();
		internalWindowView.each([&registry](entt::entity windowEntity, WindowInternal& window) {
			glfwDestroyWindow(window.window);
			window.window = nullptr;
			registry.remove<WindowInternal>(windowEntity);
		});
	}

	void GLFWSystem::tickSystem(entt::registry& registry) {
		glfwPollEvents();

		tickCreateWindowView(registry);
		tickCloseWindowView(registry);
	}

	void GLFWSystem::tickCreateWindowView(entt::registry& registry) {
		auto initWindowInternalView = registry.view<const Core::Window>(entt::exclude<Core::WindowInternal>);
		initWindowInternalView.each([&registry](entt::entity windowEntity, const Core::Window& window) {
			GLFWwindow* internalWindow = glfwCreateWindow(window.width, window.height, window.title.c_str(), nullptr, nullptr);
			if (!internalWindow) {
				return;
			}

			registry.emplace<WindowInternal>(windowEntity, internalWindow);
		});
	}

	void GLFWSystem::tickCloseWindowView(entt::registry& registry) {
		auto internalWindowView = registry.view<const WindowInternal>();
		internalWindowView.each([&registry](const WindowInternal& window) {
			if (glfwWindowShouldClose(window.window) != 0) {
				if (registry.view<const QuitAppRequest>().empty()) {
					const entt::entity requestEntity = registry.create();
					registry.emplace<QuitAppRequest>(requestEntity, std::chrono::steady_clock::now());
				}
			}
		});
	}

} // Core
