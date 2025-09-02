
module;

#include <chrono>

#include <entt/entt.hpp>
#include <GLFW/glfw3.h>

#if GLFW_VERSION_MINOR < 2
#error "GLFW 3.2 or later is required"
#endif // GLFW_VERSION_MINOR < 2

#if defined(__linux__)
#if false
#include <wayland-egl.h>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#elif defined(_WIN32) || defined(_WIN64)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif //

#include <GLFW/glfw3native.h>

module Core.GLFWSystem;

import Core.QuitAppRequest;
import Core.GLFWWindow;
import Core.NativeWindowHandles;
import Core.Window;

namespace Core {

	void* glfwNativeWindowHandle(GLFWwindow* _window) {
#if defined(__linux__)
#if false
		wl_egl_window* win_impl = (wl_egl_window*) glfwGetWindowUserPointer(_window);
		if (!win_impl) {
			int width, height;
			glfwGetWindowSize(_window, &width, &height);
			struct wl_surface* surface = (struct wl_surface*) glfwGetWaylandWindow(_window);
			if (!surface)
				return nullptr;
			win_impl = wl_egl_window_create(surface, width, height);
			glfwSetWindowUserPointer(_window, (void*) (uintptr_t) win_impl);
		}
		return (void*) (uintptr_t) win_impl;
#else
		return reinterpret_cast<void*>(glfwGetX11Window(_window));
#endif
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
		return glfwGetCocoaWindow(_window);
#elif defined(_WIN32) || defined(_WIN64)
		return glfwGetWin32Window(_window);
#elif defined(__EMSCRIPTEN__)
		return nullptr;
#else
		static_assert(false, "Unhandled platform");
#endif
	}

	void* getNativeDisplayHandle() {
#if defined(__linux__)
#if false
		return glfwGetWaylandDisplay();
#else
		return glfwGetX11Display();
#endif
#else
		return nullptr;
#endif
	}

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
		auto internalWindowView = registry.view<GLFWWindow>();
		internalWindowView.each([&registry](entt::entity windowEntity, GLFWWindow& window) {
			glfwDestroyWindow(window.window);
			window.window = nullptr;
			registry.remove<GLFWWindow>(windowEntity);
		});
	}

	void GLFWSystem::tickSystem(entt::registry& registry) {
		glfwPollEvents();

		tickCreateWindowView(registry);
		tickCloseWindowView(registry);
	}

	void GLFWSystem::tickCreateWindowView(entt::registry& registry) {
		registry.view<Window>(entt::exclude<GLFWWindow>)
			.each([&registry](entt::entity entity, const Window& window) {
				tryCreateGLFWWindow(registry, entity, window);
			});
	}

	void GLFWSystem::tickCloseWindowView(entt::registry& registry) {
		registry.view<const GLFWWindow>()
			.each([&registry](const GLFWWindow& window) {
				if (glfwWindowShouldClose(window.window) != 0) {
					if (registry.view<const QuitAppRequest>().empty()) {
						const entt::entity requestEntity = registry.create();
						registry.emplace<QuitAppRequest>(requestEntity, std::chrono::steady_clock::now());
					}
				}
			});
	}

	void GLFWSystem::tryCreateGLFWWindow(entt::registry& registry, entt::entity entity, const Window& window) {
		GLFWwindow* internalWindow = glfwCreateWindow(window.width, window.height, window.title.c_str(), nullptr, nullptr);
		if (!internalWindow) {
			return;
		}

		registry.emplace<GLFWWindow>(entity, internalWindow);

		NativeWindowHandles nativeWindowHandles{
			glfwNativeWindowHandle(internalWindow),
			getNativeDisplayHandle()
		};

		if (!nativeWindowHandles.windowHandle) {
			return;
		}

		registry.emplace<NativeWindowHandles>(entity, std::move(nativeWindowHandles));
	}

} // Core
