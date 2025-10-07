module;

#include <filesystem>
#include <fstream>

#include <tracy/Tracy.hpp>

module Ortha.Core.FileLoadSystem;

import Ortha.Core.FileDescriptor;
import Ortha.Core.FileLoadRequest;
import Ortha.Core.Log;
import Ortha.Core.ProcessError;
import Ortha.Core.RawDataResource;
import entt;

namespace Ortha::Core {

	FileLoadSystem::FileLoadSystem(entt::registry& registry, Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	FileLoadSystem::~FileLoadSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void FileLoadSystem::tickSystem(entt::registry& registry) {
		ZoneScopedN("FileLoadSystem::tickSystem");

		auto fileResourceRequestView = registry.view<FileDescriptor, FileLoadRequest>(entt::exclude<RawDataResource, ProcessError>);
		fileResourceRequestView.each([&registry](entt::entity entity, FileDescriptor& fileDescriptor) {
			if (fileDescriptor.filePath.empty()) {
				addProcessError(registry, entity, "FileLoadRequest: Empty file path.");
				return;
			}

			if (!std::filesystem::exists(fileDescriptor.filePath)) {
				addProcessError(registry, entity, "FileLoadRequest: Path '{}' doesn't exist.", fileDescriptor.filePath);
				return;
			}

			if (std::filesystem::is_directory(fileDescriptor.filePath)) {
				addProcessError(registry, entity, "FileLoadRequest: Path '{}' is a directory.", fileDescriptor.filePath);
				return;
			}

			uintmax_t fileSize = std::filesystem::file_size(fileDescriptor.filePath);

			std::vector<uint8_t> rawData;
			rawData.reserve(fileSize);

			registry.emplace<RawDataResource>(entity, std::move(rawData), fileSize);
		});

		auto fileResourceView = registry.view<FileDescriptor, RawDataResource, FileLoadRequest>();
		fileResourceView.each(
			[&registry](entt::entity entity, FileDescriptor& fileDescriptor, RawDataResource& resource) {
				size_t remainingSize = resource.size - resource.rawData.size();
				if (remainingSize == 0) {
					registry.remove<FileLoadRequest>(entity);
					return;
				}

				size_t currentWritePos = resource.rawData.size();

				std::ifstream fileStream(fileDescriptor.filePath, std::ios::binary);
				fileStream.seekg(currentWritePos);

				char* writeHead = reinterpret_cast<char*>(&resource.rawData.data()[currentWritePos]);

				size_t readCount = std::min(1000zu, remainingSize);
				resource.rawData.resize(currentWritePos + readCount);
				fileStream.read(writeHead, readCount);
			});
	}

} // namespace Ortha::Core
