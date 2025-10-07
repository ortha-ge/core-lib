module;

#include <cstdint>
#include <vector>

export module Ortha.Core.RawDataResource;

export namespace Ortha::Core {

	struct RawDataResource {
		std::vector<uint8_t> rawData;
		uintmax_t size{};
	};

} // namespace Ortha::Core
