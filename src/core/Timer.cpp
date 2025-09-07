module;

#include <algorithm>
#include <chrono>
#include <utility>

module Core.Timer;

namespace Core::TimerInternal {
	constexpr uint32_t TimerSamplesMax = 60;
} // namespace Core::TimerInternal

namespace Core {

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
		using DeltaTimeCast = std::chrono::duration<float>;
		auto clockNow = std::chrono::steady_clock::now();
		auto deltaT = std::chrono::duration_cast<DeltaTimeCast>(clockNow - mLastTick).count();
		mLastTick = clockNow;

		mDeltaTimes.push_front(deltaT);
		if (mDeltaTimes.size() > TimerSamplesMax) {
			mDeltaTimes.pop_back();
		}

		mDeltaT = std::ranges::fold_left(mDeltaTimes, 0.0f, std::plus<float>());
		mDeltaT /= mDeltaTimes.size();
	}

	float Timer::getDeltaT() const {
		return mDeltaT;
	}

} // namespace Core
