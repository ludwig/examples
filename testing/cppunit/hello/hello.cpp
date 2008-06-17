#include <iostream>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

class Test : public CPPUNIT_NS::TestCase
{
CPPUNIT_TEST_SUITE(Test);
CPPUNIT_TEST(testHelloWorld);
CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void) {}
    void tearDown(void) {}

protected:
    void testHelloWorld(void)
    {
        //std::cout << "Hello, World!" << std::endl;
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(Test);

int main(int ac, char **av)
{
    //--- Create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    //--- Add a listener that collects the test results
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    //--- Add a listener tha tprints dots as test runs
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    //--- Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    return result.wasSuccessful() ? 0 : 1;
}
