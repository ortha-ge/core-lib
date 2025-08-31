module;

#include <chrono>

export module Core.QuitAppRequest;

export namespace Core {

struct QuitAppRequest {
	std::chrono::steady_clock::time_point timestamp{};
};

} // Core
