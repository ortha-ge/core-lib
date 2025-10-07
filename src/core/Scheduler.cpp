module;

#include <algorithm>
#include <functional>
#include <memory>

#include <tracy/Tracy.hpp>

module Ortha.Core.Scheduler;

namespace Core {

	Task::Task(std::function<void()> onTick, uint8_t priority)
		: mOnTick{ std::move(onTick) }
		, mPriority{ priority } {}

	void Task::tick() { mOnTick(); }

	void Task::setIsRemoved(bool isRemoved) { mIsRemoved = isRemoved; }

	uint8_t Task::getPriority() const { return mPriority; }

	bool Task::getIsRemoved() const { return mIsRemoved; }

	void Schedule::tick() {
		ZoneScoped;
		for (const auto& task : mTasks) {
			if (!task->getIsRemoved()) {
				task->tick();
			}
		}

		const bool isQueueModified = !mAddQueue.empty() || !mRemoveQueue.empty();
		for (; !mAddQueue.empty(); mAddQueue.pop()) {
			mTasks.emplace_back(std::move(mAddQueue.front()));
		}

		for (; !mRemoveQueue.empty(); mRemoveQueue.pop()) {
			auto task = mRemoveQueue.front();
			mTasks.erase(std::ranges::remove(mTasks, task).begin(), mTasks.end());
		}

		if (isQueueModified) {
			mTasks.sort([](const auto& lhs, const auto& rhs) { return lhs->getPriority() > rhs->getPriority(); });
		}
	}

	std::weak_ptr<Task> Schedule::schedule(std::function<void()> onTick, uint8_t priority) {
		auto task = std::make_shared<Task>(std::move(onTick), priority);
		mAddQueue.emplace(task);
		return task;
	}

	void Schedule::unschedule(std::shared_ptr<Task> task) {
		if (task) {
			task->setIsRemoved(true);
			mRemoveQueue.emplace(std::move(task));
		}
	}

	TaskHandle::TaskHandle() = default;

	TaskHandle::TaskHandle(std::weak_ptr<Schedule> schedule, std::weak_ptr<Task> task)
		: mSchedule{ std::move(schedule) }
		, mTask{ std::move(task) } {}

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

	TaskHandle Scheduler::schedule(std::function<void()> onTick, uint8_t priority) {
		auto task = mSchedule->schedule(std::move(onTick), priority);
		return { mSchedule, std::move(task) };
	}

	void Scheduler::unschedule(TaskHandle taskHandle) { mSchedule->unschedule(taskHandle.getTask()); }

} // namespace Core
