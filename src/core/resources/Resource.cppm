module;

#include <string>

export module Core.Resource;

import entt;

export namespace Core {

	class Resource {
	public:

		Resource(std::string filePath, entt::entity resourceEntity);

		[[nodiscard]] const std::string& getFilePath() const;
		[[nodiscard]] entt::entity getResourceEntity() const;

	private:

		std::string mFilePath;
		entt::entity mResourceEntity{ entt::null };

	};

} // namespace Core
