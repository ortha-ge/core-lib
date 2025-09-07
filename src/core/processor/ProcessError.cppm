module;

#include <chrono>
#include <cstdint>
#include <string>
#include <variant>

export module Core.ProcessError;

export namespace Core {

	struct ProcessError {
		using RetryTimepoint = std::chrono::steady_clock::time_point;

		std::string errorMessage{};
		std::variant<uint32_t, RetryTimepoint, bool> retry;
	};

} // namespace Core
