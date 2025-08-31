module;

#include <bx/allocator.h>
#include <entt/fwd.hpp>

export module Core.ImageLoadSystem;

export namespace Core {

	class ImageLoadSystem {
	public:

		ImageLoadSystem();

		static void initSystem(entt::registry& registry);
		static void destroySystem(entt::registry& registry);
		static void tickSystem(entt::registry& registry);

	private:

		std::unique_ptr<bx::AllocatorI> mAllocator{};

	};

} // Core
