module;

#include <cstdint>
#include <vector>

export module Ortha.Core.RawDataResource;

export namespace Core {

	struct RawDataResource {
		std::vector<uint8_t> rawData;
		uintmax_t size{};
	};

} // namespace Core
