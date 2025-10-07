module;

#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <queue>

export module Ortha.Core.Scheduler;

export namespace Core {

	class Task {
	public:
		Task(std::function<void()> onTick, uint8_t priority);

		void tick();

		void setIsRemoved(bool isRemoved);

		uint8_t getPriority() const;
		bool getIsRemoved() const;

	private:
		std::function<void()> mOnTick;
		uint8_t mPriority;
		bool mIsRemoved{ false };
	};

	class Schedule {
	public:
		void tick();

		std::weak_ptr<Task> schedule(std::function<void()> onTick, uint8_t priority);
		void unschedule(std::shared_ptr<Task> task);

	private:

		std::list<std::shared_ptr<Task>> mTasks;
		std::queue<std::shared_ptr<Task>> mAddQueue;
		std::queue<std::shared_ptr<Task>> mRemoveQueue;
	};

	class TaskHandle {
	public:
		TaskHandle();
		TaskHandle(std::weak_ptr<Schedule> schedule, std::weak_ptr<Task> task);
		~TaskHandle();

		TaskHandle(TaskHandle&&) noexcept;
		TaskHandle& operator=(TaskHandle&&) noexcept;

		TaskHandle(const TaskHandle&) = delete;
		TaskHandle& operator=(const TaskHandle&) = delete;

		std::shared_ptr<Task> getTask() const;

	private:
		std::weak_ptr<Schedule> mSchedule;
		std::weak_ptr<Task> mTask;
	};

	class Scheduler {
	public:
		Scheduler();

		void tick();

		TaskHandle schedule(std::function<void()> onTick, uint8_t priority = 0);
		void unschedule(TaskHandle taskHandle);

	private:
		std::shared_ptr<Schedule> mSchedule;
	};

} // namespace Core
