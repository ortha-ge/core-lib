module;

#include <algorithm>
#include <functional>
#include <memory>

module Core.Scheduler;

namespace Core {

	Task::Task(std::function<void()> onTick)
		: mOnTick{ std::move(onTick) } {}

	void Task::tick() { mOnTick(); }

	void Schedule::tick() {
		for (auto&& task : mTasks) {
			task->tick();
		}
	}

	std::weak_ptr<Task> Schedule::schedule(std::function<void()> onTick) {
		auto task = std::make_shared<Task>(std::move(onTick));
		mTasks.emplace_back(task);
		return task;
	}

	void Schedule::unschedule(std::shared_ptr<Task> task) {
		if (task) {
			mTasks.erase(std::remove(mTasks.begin(), mTasks.end(), task), mTasks.end());
		}
	}

	TaskHandle::TaskHandle() = default;

	TaskHandle::TaskHandle(std::weak_ptr<Schedule> schedule, std::weak_ptr<Task> task)
		: mSchedule{ schedule }
		, mTask{ task } {}

	TaskHandle::~TaskHandle() {
		auto schedule = mSchedule.lock();
		auto task = mTask.lock();
		if (schedule && task) {
			schedule->unschedule(std::move(task));
		}
	}

	TaskHandle::TaskHandle(TaskHandle&&) noexcept = default;
	TaskHandle& TaskHandle::operator=(TaskHandle&&) noexcept = default;

	std::shared_ptr<Task> TaskHandle::getTask() const { return mTask.lock(); }

	Scheduler::Scheduler()
		: mSchedule{ std::make_shared<Schedule>() } {}

	void Scheduler::tick() { mSchedule->tick(); }

	TaskHandle Scheduler::schedule(std::function<void()> onTick) {
		auto task = mSchedule->schedule(std::move(onTick));
		return { mSchedule, std::move(task) };
	}

	void Scheduler::unschedule(TaskHandle taskHandle) { mSchedule->unschedule(taskHandle.getTask()); }

} // namespace Core
