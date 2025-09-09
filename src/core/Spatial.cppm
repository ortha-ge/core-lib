module;

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

export module Core.Spatial;

export namespace Core {

	struct Spatial {
		glm::vec3 position{};
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::fquat rotation{};
	};

} // namespace Core
