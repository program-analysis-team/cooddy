/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef COODDY_ANALYZER_SOURCE_SOLVER_MEMORYMONITOR_H_
#define COODDY_ANALYZER_SOURCE_SOLVER_MEMORYMONITOR_H_

#include <utils/Log.h>
#include <z3++.h>

#include <atomic>
#include <shared_mutex>

// LCOV_EXCL_START
class MemoryMonitor {
public:
    MemoryMonitor(bool useMemMonitor)
    {
        if (useMemMonitor) {
            myMutex.lock_shared();
            myWasLocked = true;
        }
    }

    ~MemoryMonitor()
    {
        if (!myWasLocked) {
            return;
        }
        myMutex.unlock_shared();
        if (myNeedResetMemory) {
            std::unique_lock lock(myMutex);
            if (myNeedResetMemory) {
                Log(LogLevel::WARNING) << "Reset z3 memory " << std::endl;
                Z3_reset_memory();
                myNeedResetMemory = false;
            }
        }
    }

    void Check(z3::solver& solver)
    {
        static uint32_t memoryLimit = 2000;
        static std::atomic<uint32_t> checkCounter = 0;
        if ((checkCounter++ % memoryLimit) != 0) {
            return;
        }
        auto stat = solver.statistics();
        for (auto i = 0; i < stat.size(); ++i) {
            if (stat.key(i) == "memory" && stat.double_value(i) > memoryLimit) {
                Log(LogLevel::WARNING) << "Need to reset z3 memory " << stat << std::endl;
                myNeedResetMemory = true;
            }
        }
    }

private:
    bool myWasLocked = false;
    static std::shared_mutex myMutex;
    static bool myNeedResetMemory;
};
// LCOV_EXCL_STOP

#endif  // COODDY_ANALYZER_SOURCE_SOLVER_MEMORYMONITOR_H_
