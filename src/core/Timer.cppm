module;

#include <chrono>
#include <list>

export module Core.Timer;

import Core.Scheduler;

export namespace Core {

	class Timer {
	public:

		Timer(Scheduler&);
		~Timer();

		void tick();

		[[nodiscard]] float getDeltaT() const;

	private:

		Scheduler& mScheduler;
		TaskHandle mTickHandle;
		std::chrono::steady_clock::time_point mLastTick;

		std::list<float> mDeltaTimes;
		float mDeltaT{ 0.0f };

	};

} // namespace Core
