module;

#include <chrono>

export module Ortha.Core.QuitAppRequest;

export namespace Ortha::Core {

	struct QuitAppRequest {
		std::chrono::steady_clock::time_point timestamp{};
	};

} // namespace Ortha::Core
