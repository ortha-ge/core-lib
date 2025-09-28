module;

#include <chrono>
#include <variant>

#include <tracy/Tracy.hpp>

module Core.ProcessErrorRetrySystem;

import Core.ProcessError;
import Core.ProcessErrorLoggedFlag;
import entt;

namespace Core::ProcessErrorRetrySystemInternal {
	bool tickProcessError(entt::registry& registry, const entt::entity entity, ProcessError& error, bool shouldRetry) {
		return shouldRetry;
	}

	bool tickProcessError(entt::registry& registry, const entt::entity entity, ProcessError& error, uint32_t remainingTicks) {
		if (remainingTicks > 0) {
			--remainingTicks;
		}

		error.retry = remainingTicks;
		return remainingTicks == 0u;
	}

	bool tickProcessError(entt::registry& registry, const entt::entity entity, ProcessError& error, ProcessError::RetryTimepoint retryTimePoint) {
		return std::chrono::steady_clock::now() >= retryTimePoint;
	}
} // Core::ProcessErrorRetrySystemInternal

namespace Core {

	ProcessErrorRetrySystem::ProcessErrorRetrySystem(EnTTRegistry& registry, Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {

		mTickHandle = mScheduler.schedule([this]() {
			tickSystem(mRegistry);
		});
	}

	ProcessErrorRetrySystem::~ProcessErrorRetrySystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void ProcessErrorRetrySystem::tickProcessError(entt::registry& registry, entt::entity entity, ProcessError& processError) {
		bool shouldRetry = std::visit([&registry, entity, &processError](auto&& retry) {
			return ProcessErrorRetrySystemInternal::tickProcessError(registry, entity, processError, retry);
		}, processError.retry);

		if (shouldRetry) {
			registry.remove<ProcessError>(entity);
			registry.remove<ProcessErrorLoggedFlag>(entity);
		}
	}

	void ProcessErrorRetrySystem::tickSystem(entt::registry& registry) {
		ZoneScopedN("ProcessErrorRetrySystem::tickSystem");

		registry.view<ProcessError>()
			.each([this, &registry](const entt::entity entity, ProcessError& processError) {
				tickProcessError(registry, entity, processError);
			});
	}

} // namespace Core
