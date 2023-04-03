#include <condition_variable>
#include <mutex>

class A {
public:
    std::condition_variable myCv;
    std::mutex myMutex;
    void test1()
    {
        std::unique_lock<std::mutex> lock(myMutex);
        myCv.wait(lock, [&] { return true; });
    }

    void test2()
    {
        myCv.notify_one();
    }

    void test3()
    {
        myCv.notify_all();
    }
};
