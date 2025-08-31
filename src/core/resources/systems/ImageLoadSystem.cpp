module;

#include <vector>

#include <bimg/decode.h>
#include <bx/error.h>
#include <entt/entt.hpp>

module Core.ImageLoadSystem;

import Core.FileLoadRequest;
import Core.ImageDescriptor;
import Core.ImageResource;
import Core.RawDataResource;

namespace Core {

	ImageLoadSystem::ImageLoadSystem()
		: mAllocator{ std::make_unique<bx::DefaultAllocator>() } {
	}

	void ImageLoadSystem::initSystem(entt::registry& registry) {
	}

	void ImageLoadSystem::destroySystem(entt::registry& registry) {

	}

	void ImageLoadSystem::tickSystem(entt::registry& registry) {
		auto imageResourceView = registry.view<const ImageDescriptor, const RawDataResource>(entt::exclude<FileLoadRequest, ImageResource>);
		imageResourceView.each([&registry](entt::entity entity, const ImageDescriptor& imageDescriptor, const RawDataResource& rawDataResource) {
			bx::DefaultAllocator allocator{};
			bx::Error error{};
			bimg::ImageContainer* image = bimg::imageParse(&allocator, rawDataResource.rawData.data(), rawDataResource.size, bimg::TextureFormat::Enum::RGBA32F, &error);
			if (image) {
				uint8_t* readHead = reinterpret_cast<uint8_t*>(image->m_data);
				registry.emplace<ImageResource>(entity,
					std::vector<uint8_t>(readHead, &readHead[image->m_size]),
					static_cast<uint32_t>(image->m_format),
					image->m_size,
					image->m_offset,
					image->m_width,
					image->m_height,
					image->m_depth,
					image->m_numLayers,
					image->m_numMips,
					image->m_hasAlpha,
					image->m_cubeMap,
					image->m_ktx,
					image->m_ktxLE,
					image->m_pvr3,
					image->m_srgb
				);
				bimg::imageFree(image);
				image = nullptr;
			} else {
				printf("Error: %s\n", error.getMessage().getCPtr());
			}
		});
	}

} // Core
