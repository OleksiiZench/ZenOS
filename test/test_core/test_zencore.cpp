#include <unity.h>
#include <memory>
#include <core/ZenCore.h>
#include <core/IModule.h>

class TestModule : public IModule
{
public:
    bool init_called = false;
    bool update_called = false;

    virtual void init() override
    {
        init_called = true;
    }

    virtual void update() override
    {
        update_called = true;
    }
};

void setUp(void)
{

}

void tearDown(void)
{

}

void test_zencore_registers_and_runs_modules(void)
{
    ZenCore core;

    auto module_ptr = std::make_unique<TestModule>();
    TestModule* raw_module_ptr = module_ptr.get();

    TEST_ASSERT_FALSE(raw_module_ptr->init_called);
    TEST_ASSERT_FALSE(raw_module_ptr->update_called);

    core.registerModule(std::move(module_ptr));

    core.init();
    TEST_ASSERT_TRUE(raw_module_ptr->init_called);

    core.update();
    TEST_ASSERT_TRUE(raw_module_ptr->update_called);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_zencore_registers_and_runs_modules);

    return UNITY_END();
}
