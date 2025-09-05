
#include <GLFW/glfw3.h>
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>

import Core.GLFWSystem;
import Core.QuitAppRequest;
import Core.GLFWWindow;
import Core.NativeWindowHandles;
import Core.Window;

TEST_CASE("Window_GLFWSystemTickCreateWindowView_GLFWWindowProduced", "GLFWSystem") {
	using namespace Core;
	const std::string_view expectedWindowTitle = "Test Window";
	const uint32_t expectedWindowWidth = 640;
	const uint32_t expectedWindowHeight = 480;
	entt::registry registry{};
	entt::entity windowEntity = registry.create();
	registry.emplace<Window>(
		windowEntity, std::string{ expectedWindowTitle }, expectedWindowWidth, expectedWindowHeight);
	GLFWSystem::initSystem(registry);

	GLFWSystem::tickCreateWindowView(registry);

	REQUIRE(registry.all_of<GLFWWindow>(windowEntity));
	const auto& glfwWindow = registry.get<GLFWWindow>(windowEntity);
	REQUIRE(glfwWindow.window != nullptr);
	const char* actualWindowTitle = glfwGetWindowTitle(glfwWindow.window);
	REQUIRE(actualWindowTitle != nullptr);
	REQUIRE(expectedWindowTitle == actualWindowTitle);
	int actualWindowWidth{};
	int actualWindowHeight{};
	glfwGetWindowSize(glfwWindow.window, &actualWindowWidth, &actualWindowHeight);
	REQUIRE(expectedWindowWidth == actualWindowWidth);
	REQUIRE(expectedWindowHeight == actualWindowHeight);
}

TEST_CASE("WindowInternal_GLFWSystemTickCloseWindowView_QuitAppRequestProduced", "GLFWSystem") {
	using namespace Core;
	const bool isInitialized = glfwInit();
	REQUIRE(isInitialized == GLFW_TRUE);
	entt::registry registry{};
	entt::entity windowInternalEntity = registry.create();
	GLFWwindow* glfwWindow = glfwCreateWindow(640, 480, "Test Window", nullptr, nullptr);
	REQUIRE(glfwWindow != nullptr);
	registry.emplace<GLFWWindow>(windowInternalEntity, glfwWindow);
	glfwSetWindowShouldClose(glfwWindow, true);

	GLFWSystem::tickCloseWindowView(registry);

	const auto quitAppRequests = registry.view<QuitAppRequest>();
	REQUIRE(quitAppRequests.size() == 1);
}
