
#include <catch2/catch_test_macros.hpp>
#include <entt/entt.hpp>
#include <GLFW/glfw3.h>

import Core.GLFWSystem;
import Core.QuitAppRequest;
import Core.Window;
import Core.WindowInternal;

TEST_CASE("Window_GLFWSystemTickCreateWindowView_WindowInternalProduced", "GLFWSystem") {
    using namespace Core;
    entt::registry registry{};
    entt::entity windowEntity = registry.create();
    registry.emplace<Window>(windowEntity, "Test Window", 640, 480);
    GLFWSystem::initSystem(registry);

    GLFWSystem::tickCreateWindowView(registry);

    const bool hasWindowInternal = registry.all_of<WindowInternal>(windowEntity);
    REQUIRE(hasWindowInternal);
    const WindowInternal& windowInternal = registry.get<WindowInternal>(windowEntity);
    REQUIRE(windowInternal.window != nullptr);
}

TEST_CASE("WindowInternal_GLFWSystemTickCloseWindowView_QuitAppRequestProduced", "GLFWSystem") {
    using namespace Core;
    const bool isInitialized = glfwInit();
    REQUIRE(isInitialized == GLFW_TRUE);
    entt::registry registry{};
    entt::entity windowInternalEntity = registry.create();
    GLFWwindow* glfwWindow = glfwCreateWindow(640, 480, "Test Window", nullptr, nullptr);
    REQUIRE(glfwWindow != nullptr);
    registry.emplace<WindowInternal>(windowInternalEntity, glfwWindow);
    glfwSetWindowShouldClose(glfwWindow, true);

    GLFWSystem::tickCloseWindowView(registry);

    const auto quitAppRequests = registry.view<QuitAppRequest>();
    REQUIRE(quitAppRequests.size() == 1);
}
