module;

#include <functional>
#include <memory>
#include <vector>

export module Core.Scheduler;

export namespace Core {

	class Task {
	public:
		Task(std::function<void()> onTick);

		void tick();

	private:
		std::function<void()> mOnTick;
	};

	class Schedule {
	public:
		void tick();

		std::weak_ptr<Task> schedule(std::function<void()> onTick);
		void unschedule(std::shared_ptr<Task> task);

	private:
		std::vector<std::shared_ptr<Task>> mTasks;
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

		TaskHandle schedule(std::function<void()> onTick);
		void unschedule(TaskHandle taskHandle);

	private:
		std::shared_ptr<Schedule> mSchedule;
	};

} // namespace Core
