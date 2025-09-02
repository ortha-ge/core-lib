module;

#include <entt/fwd.hpp>

export module Core.ResourceHandle;

export namespace Core {

	class ResourceHandle {
	public:

		ResourceHandle();
		ResourceHandle(entt::entity resourceEntity);
		~ResourceHandle();

		ResourceHandle(const ResourceHandle& other);
		ResourceHandle(ResourceHandle&& other) noexcept;

		ResourceHandle& operator=(const ResourceHandle& other);
		ResourceHandle& operator=(ResourceHandle&& other) noexcept;



	//private:

		entt::entity mResourceEntity;

	};

} // namespace Core
