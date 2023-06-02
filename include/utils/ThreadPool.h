/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
//
// class ThreadPool implements distributed tasks completion
//
#ifndef COODDY_ANALYZER_INCLUDE_THREADPOOL_H_
#define COODDY_ANALYZER_INCLUDE_THREADPOOL_H_

#include <utils/Timer.h>

#include <condition_variable>
#include <deque>
#include <future>
#include <thread>
#include <vector>

namespace HCXX {
extern uint32_t GetMaxThreadsCount();

class ThreadPool {
public:
    static const uint32_t MAX_PRIORITY = UINT32_MAX;

    struct Task {
        virtual void Start() = 0;
        // LCOV_EXCL_START
        virtual uint32_t GetPriority()
        {
            return ThreadPool::MAX_PRIORITY;
        }
        // LCOV_EXCL_STOP

        virtual ~Task() = default;
    };

    void AddTask(std::shared_ptr<Task> task)
    {
        std::unique_lock<std::mutex> lock(myMutex);
        // LCOV_EXCL_START
        if (myExitState) {
            return;
        }
        // LCOV_EXCL_STOP
        myTasks.emplace_back(task);
        ++myTasksCounter;
        if (myTasks.size() > myThreads.size() && myThreads.size() < myMaxThreads) {
            myThreads.emplace_back(std::make_unique<std::thread>([&](void*) { ThreadFunc(); }, nullptr));
        }
        myCv.notify_one();
    }

    void StopAll()
    {
        {
            std::unique_lock<std::mutex> lock(myMutex);
            myExitState = true;
        }
        myCv.notify_all();
        for (auto& thread : myThreads) {
            thread->join();
        }
    }

    void WaitAll()
    {
        while (myTasksCounter != 0) {
            std::this_thread::yield();
        }
    }

    uint64_t GetRunTime() const
    {
        return myRunTime;
    }

    explicit ThreadPool(uint32_t maxThreads = 0) : myMaxThreads(maxThreads != 0 ? maxThreads : GetMaxThreadsCount()) {}
    ~ThreadPool()
    {
        StopAll();
    }

private:
    void ThreadFunc()
    {
        while (!myExitState) {
            std::shared_ptr<Task> task;
            {
                std::unique_lock<std::mutex> lock(myMutex);
                myCv.wait(lock, [&] { return !myTasks.empty() || myExitState; });
                if (myExitState) {
                    return;
                }
                task = PopTask();
            }
            Timer timer;
            task->Start();
            timer.Flush(myRunTime);
            myTasksCounter--;
        }
    }

    std::shared_ptr<Task> PopTask()
    {
        uint32_t maxPos = 0;
        uint32_t maxPriority = 0;
        for (auto i = 0; i < myTasks.size() && maxPriority != MAX_PRIORITY; ++i) {
            uint32_t priority = myTasks[i]->GetPriority();
            if (priority > maxPriority) {
                // LCOV_EXCL_START
                maxPriority = priority;
                maxPos = i;
                // LCOV_EXCL_STOP
            }
        }
        auto result = myTasks[maxPos];
        myTasks.erase(myTasks.begin() + maxPos);
        return result;
    }

    std::mutex myMutex;
    std::condition_variable myCv;
    std::deque<std::shared_ptr<Task>> myTasks;
    std::vector<std::unique_ptr<std::thread>> myThreads;
    std::atomic<uint64_t> myRunTime = 0;
    std::atomic<uint32_t> myTasksCounter = 0;
    uint32_t myMaxThreads;
    bool myExitState = false;
};

class PoolTask {
public:
    explicit PoolTask(ThreadPool& pool) : myThreadPool(pool) {}

    virtual ~PoolTask() = default;

    // LCOV_EXCL_START
    virtual void OnTaskCompleted(){};
    // LCOV_EXCL_STOP

    void AddTask(std::shared_ptr<ThreadPool::Task> task)
    {
        ++myTaskCounter;
        myThreadPool.AddTask(task);
    }

    void ReleaseTask()
    {
        if (--myTaskCounter == 0) {
            OnTaskCompleted();
        }
    }

private:
    ThreadPool& myThreadPool;
    std::atomic<uint32_t> myTaskCounter = 1;
};

class PoolSubTask : public HCXX::ThreadPool::Task {
public:
    PoolSubTask(PoolTask& task, const std::function<void()>& callback, uint32_t priority = ThreadPool::MAX_PRIORITY)
        : myTask(task), myPriority(priority), myCallback(callback)
    {}
    ~PoolSubTask() = default;

private:
    void Start() override
    {
        myCallback();
        myTask.ReleaseTask();
    }
    uint32_t GetPriority() override
    {
        return myPriority;
    }

private:
    std::function<void()> myCallback;
    PoolTask& myTask;
    uint32_t myPriority;
};

};      // namespace HCXX
#endif  // COODDY_ANALYZER_INCLUDE_THREADPOOL_H_
