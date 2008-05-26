#include "TestStudent.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextOutputter.h>


// Register test fixture
CPPUNIT_TEST_SUITE_REGISTRATION(StudentTestCase);


// method to test the constructor
void StudentTestCase::testConstructor()
{
    // create a student object
    Student stu("Tan Meng Chee", "94-1111B-13");

    // check that the object is constructed correctly
    std::string student_name = stu.getStuName();
    CPPUNIT_ASSERT(student_name == "Tan Meng Chee");
    std::string student_number = stu.getStuNumber();
    CPPUNIT_ASSERT(student_number == "94-1111B-13");
}


// method to test the assigning and retrieval of grades
void StudentTestCase::testAssignAndRetrieveGrades()
{
    // create a student
    Student stu("Jimmy", "946302B");

    // assign a few grades to this student
    stu.assignGrade("cs2102", 60);
    stu.assignGrade("cs2103", 70);
    stu.assignGrade("cs3214s", 80);

    // verify that the assignment is correct
    CPPUNIT_ASSERT_EQUAL(60, stu.getGrade("cs2102"));
    CPPUNIT_ASSERT_EQUAL(70, stu.getGrade("cs2103"));

    // attempt to retrieve a course that does not exist
    CPPUNIT_ASSERT_EQUAL(-1, stu.getGrade("cs21002"));
}




// the main method
int main(int argc, char *argv[])
{
    CppUnit::TestResultCollector result;

    CppUnit::TestResult controller;
    controller.addListener(&result);

    CppUnit::BriefTestProgressListener progress;
    controller.addListener(&progress);

    CppUnit::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    CppUnit::TextOutputter outputter(&result, std::cerr);
    outputter.write();

    return result.wasSuccessful() ? 0 : 1;
}
