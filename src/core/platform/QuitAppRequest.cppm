module;

#include <chrono>

export module Ortha.Core.QuitAppRequest;

export namespace Core {

	struct QuitAppRequest {
		std::chrono::steady_clock::time_point timestamp{};
	};

} // namespace Core
