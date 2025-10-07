module;

#include <algorithm>
#include <chrono>
#include <utility>

module Ortha.Core.Timer;

namespace Ortha::Core::TimerInternal {
	constexpr uint32_t TimerSamplesMax = 60;
} // namespace Ortha::Core::TimerInternal

namespace Ortha::Core {

	Timer::Timer(Scheduler& scheduler)
		: mScheduler{ scheduler } {

		mLastTick = std::chrono::steady_clock::now();
		mTickHandle = mScheduler.schedule([this] {
			tick();
		});
	}

	Timer::~Timer() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void Timer::tick() {
		using namespace TimerInternal;

		if (getIsPaused()) {
			return;
		}

		using DeltaTimeCast = std::chrono::duration<float>;
		auto clockNow = std::chrono::steady_clock::now();
		auto deltaT = std::chrono::duration_cast<DeltaTimeCast>(clockNow - mLastTick).count();
		mLastTick = clockNow;

		mDeltaTimes.push_front(deltaT);
		if (mDeltaTimes.size() > TimerSamplesMax) {
			mDeltaTimes.pop_back();
		}

		mDeltaT = std::ranges::fold_left(mDeltaTimes, 0.0f, std::plus<>());
		mDeltaT /= static_cast<float>(mDeltaTimes.size());
	}

	void Timer::setIsPaused(bool isPaused) {
		if (mIsPaused == isPaused) {
			return;
		}

		mIsPaused = isPaused;

		if (isPaused) {
			mDeltaTimes.clear();
			mDeltaT = 0.0f;
		} else {
			mLastTick = std::chrono::steady_clock::now();
		}
	}

	float Timer::getDeltaT() const {
		return mDeltaT;
	}

	bool Timer::getIsPaused() const {
		return mIsPaused;
	}

} // namespace Ortha::Core
