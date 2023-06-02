/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#ifndef CONTEXT_POOL_HEADER_FILE
#define CONTEXT_POOL_HEADER_FILE

#include <memory>
#include <mutex>
#include <vector>

namespace HCXX {

template <typename Factory>
class ContextPool;

template <typename Factory>
class ContextHolder {
public:
    using Context = typename Factory::FactoryObject;

public:
    explicit ContextHolder(std::unique_ptr<Context>&& context, ContextPool<Factory>* pool)
        : myContext(std::move(context)), myPool(pool)
    {}

    ContextHolder(ContextHolder&& other) noexcept
        : myContext(std::move(other.myContext)), myPool(std::move(other.myPool))
    {}

    ContextHolder& operator=(ContextHolder&& other) noexcept
    {
        ContextHolder tmp(std::move(other));
        this->Swap(tmp);
        return *this;
    }

    void Swap(ContextHolder& other) noexcept
    {
        std::swap(myContext, other.myContext);
        std::swap(myPool, other.myPool);
    }

    ContextHolder(const ContextHolder& other) = delete;
    ContextHolder& operator=(const ContextHolder& other) = delete;

    Context* operator->()
    {
        return myContext.get();
    }

    Context* Get()
    {
        return myContext.get();
    }

    operator bool() const
    {
        return static_cast<bool>(myContext);
    }

    ~ContextHolder();

private:
    std::unique_ptr<Context> myContext;
    ContextPool<Factory>* myPool;
};

//  Some checkers require contexts which creation is rather expensive.
//  The goal of the template is to provide pool to let them reuse such contexts
//  The example:
//      The client should provide three structs(Context, Configuration and ContextFactory)
//      struct Context {};  //    The expensive context
//
//      struct CheckerConfiguration {};    //  The configuration which is used to create Context
//
//      struct ContextFactory { //  The factory which creates Context
//
//          //  It should provide three aliases for Pool
//          using FactoryObject = Context;
//          using FactoryObjectPtr = std::unique_ptr<FactoryObject>;
//          using Cfg = CheckerConfiguration;
//
//          //  It should provide Configure method
//          //  It is called only ones
//          void Configure(Cfg&& cfg) {}
//
//          //  It should provide the method which create context
//          FactoryObjectPtr CreateContext() const { return std::make_unique<FactoryObject>(); }
//      };
//
//      The checker uses it the following way(the checker's interface may differ from real one)
//      struct MyChecker {
//          //  The pool is stored as member
//          HCXX::ContextPool<ContextFactory> myPool;
//
//          void Initialize() {
//              const int preallocatedItemsInPool = ...
//              CheckerConfiguration cfg = ...
//              myPool.Configure(preallocatedItemsInPool, std::move(cfg));//    Note: it calls ContextFactory.Configure
//          }
//
//          void checkNode(...) {
//              ...
//              //  Get existing item from the pool. It will create new item using factory if the pool is empty
//              auto holder = pool.AllocateContext();
//              //  Now you may use operator '->' or method 'get' of the 'holder' object to get your context
//              ...
//              //  The context is automatically returned to the pool when 'holder' is destroyed
//          }
//      };
template <typename Factory>
class ContextPool {
public:
    explicit ContextPool(std::unique_ptr<Factory>&& factory)
        : myMutex(), myFactory(std::move(factory)), myAllocatedObjects()
    {}

    ContextPool(ContextPool&& other) = default;

    ContextPool(const ContextPool& other) = delete;
    ContextPool& operator=(const ContextPool& other) = delete;
    ContextPool& operator=(ContextPool&& other) = delete;

    ~ContextPool() = default;

public:
    ContextHolder<Factory> AllocateContext()
    {
        {
            std::lock_guard<std::mutex> guard(myMutex);
            if (!myAllocatedObjects.empty()) {
                ContextHolder<Factory> res(std::move(myAllocatedObjects.back()), this);
                myAllocatedObjects.resize(myAllocatedObjects.size() - 1);
                return res;
            }
        }

        //  LCOV_EXCL_START
        //  Coverage thinks that the lines are unreachable
        //  It is issue on the gcov side
        const Factory* factory = myFactory.get();
        return ContextHolder<Factory>(factory->CreateContext(), this);
        //  LCOV_EXCL_STOP
    }

    void AddObject(typename Factory::FactoryObjectPtr&& ptr)
    {
        std::lock_guard<std::mutex> guard(myMutex);
        myAllocatedObjects.emplace_back(std::move(ptr));
    }

    void Configure(int preallocatedAmount, typename Factory::Cfg&& cfg)
    {
        myFactory->Configure(std::move(cfg));
        myAllocatedObjects.clear();
        myAllocatedObjects.reserve(preallocatedAmount);
        for (int i = 0; i < preallocatedAmount; ++i) {
            myAllocatedObjects.emplace_back(myFactory->CreateContext());
        }
    }

private:
    std::mutex myMutex;
    std::unique_ptr<Factory> myFactory;
    std::vector<typename Factory::FactoryObjectPtr> myAllocatedObjects;
};

template <typename Factory>
ContextHolder<Factory>::~ContextHolder()
{
    if (myContext) {
        myPool->AddObject(std::move(myContext));
    }
}

}  //  namespace HCXX

#endif  //  #ifndef CONTEXT_POOL_HEADER_FILE
