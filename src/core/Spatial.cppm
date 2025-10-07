export module Ortha.Core.Spatial;

import glm;

export namespace Core {

	struct SpatialData {
		glm::vec3 position{};
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::fquat rotation{};
	};

	struct Spatial : SpatialData {};

} // namespace Core
