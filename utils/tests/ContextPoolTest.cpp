/// Copyright (C) 2020-2023 Huawei Technologies Co., Ltd.
///
/// This file is part of Cooddy, distributed under the GNU GPL version 3 with a Linking Exception.
/// For full terms see https://github.com/program-analysis-team/cooddy/blob/master/LICENSE.md
#include <utils/ContextPool.h>

#include "TestBaseClass.h"

using namespace HCXX;

// The fixture for testing class ContextPoolTest
class ContextPoolTest : public TestBaseClass {
protected:
    struct Configuration {};

    struct Context {};

    struct ContextFactory {
        using FactoryObject = Context;
        using FactoryObjectPtr = std::unique_ptr<FactoryObject>;
        using Cfg = Configuration;

        void Configure(Cfg&& cfg) {}

        FactoryObjectPtr CreateContext() const
        {
            ++createdObjectsAmount;
            return std::make_unique<FactoryObject>();
        }

        mutable int createdObjectsAmount = 0;
    };

    using TestPool = HCXX::ContextPool<ContextFactory>;
};

TEST_F(ContextPoolTest, ItemsAreReused)
{
    TestPool pool(std::make_unique<ContextFactory>());

    pool.Configure(2, Configuration());
    auto ptr1 = pool.AllocateContext();
    ASSERT_TRUE(ptr1);

    Context* raw2 = nullptr;
    {
        auto ptr2 = pool.AllocateContext();
        ASSERT_TRUE(ptr2);
        raw2 = ptr2.Get();
    }

    //  Now ptr2 should be returned to pool
    //  It should be returned if we request another context
    auto ptr3 = pool.AllocateContext();
    ASSERT_TRUE(ptr3);
    ASSERT_EQ(ptr3.Get(), raw2);
}

TEST_F(ContextPoolTest, ItemsAreCreatedIfRequired)
{
    auto factory = std::make_unique<ContextFactory>();
    ContextFactory* factoryRaw = factory.get();
    TestPool pool(std::move(factory));

    pool.Configure(2, Configuration());
    auto ptr1 = pool.AllocateContext();
    ASSERT_TRUE(ptr1);

    auto ptr2 = pool.AllocateContext();
    ASSERT_TRUE(ptr2);

    int oldAllocatedAmount = factoryRaw->createdObjectsAmount;
    auto ptr3 = pool.AllocateContext();
    ASSERT_TRUE(ptr3);
    ASSERT_EQ(oldAllocatedAmount + 1, factoryRaw->createdObjectsAmount);
}

TEST_F(ContextPoolTest, AdditionalItemsAreUsed)
{
    TestPool pool(std::make_unique<ContextFactory>());
    pool.Configure(2, Configuration());

    auto ptr1 = pool.AllocateContext();
    ASSERT_TRUE(ptr1);

    auto ptr2 = pool.AllocateContext();
    ASSERT_TRUE(ptr2);

    //  Now the pool is empty
    //  New items are created by factory
    std::set<Context*> createdItems;
    {
        auto ptr3 = pool.AllocateContext();
        ASSERT_TRUE(ptr3);
        createdItems.emplace(ptr3.Get());

        auto ptr4 = pool.AllocateContext();
        ASSERT_TRUE(ptr4);
        createdItems.emplace(ptr4.Get());

        auto ptr5 = pool.AllocateContext();
        ASSERT_TRUE(ptr5);
        createdItems.emplace(ptr5.Get());
    }

    //  ptr3, ptr34 and ptr5 are returned to pool
    //  But we get them instead of allocating new items via factory
    //  when request contexts

    {
        auto ptr3 = pool.AllocateContext();
        ASSERT_TRUE(ptr3);
        ASSERT_NE(createdItems.find(ptr3.Get()), createdItems.end());

        auto ptr4 = pool.AllocateContext();
        ASSERT_TRUE(ptr4);
        ASSERT_NE(createdItems.find(ptr4.Get()), createdItems.end());

        auto ptr5 = pool.AllocateContext();
        ASSERT_TRUE(ptr5);
        ASSERT_NE(createdItems.find(ptr5.Get()), createdItems.end());
    }
}
