module;

#include <string>

module Ortha.Core.Resource;
import entt;

namespace Core {

	Resource::Resource(std::string filePath, const entt::entity resourceEntity)
		: mFilePath{ std::move(filePath) }
		, mResourceEntity{ resourceEntity } {
	}

	const std::string& Resource::getFilePath() const {
		return mFilePath;
	}

	entt::entity Resource::getResourceEntity() const {
		return mResourceEntity;
	}

} // namespace Core
