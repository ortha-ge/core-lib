module;

#include <chrono>
#include <list>

export module Core.Timer;

import Core.Scheduler;

export namespace Core {

	class Timer {
	public:

		explicit Timer(Scheduler&);
		~Timer();

		void tick();
		void setIsPaused(bool isPaused);

		[[nodiscard]] float getDeltaT() const;
		[[nodiscard]] bool getIsPaused() const;

	private:

		Scheduler& mScheduler;
		TaskHandle mTickHandle;
		std::chrono::steady_clock::time_point mLastTick;

		std::list<float> mDeltaTimes;
		float mDeltaT{ 0.0f };
		bool mIsPaused{ false };

	};

} // namespace Core
