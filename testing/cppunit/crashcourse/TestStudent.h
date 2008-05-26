#ifndef TestStudent_h
#define TestStudent_h

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <string>

#include "Student.h"


class StudentTestCase : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(StudentTestCase);
CPPUNIT_TEST(testConstructor);
CPPUNIT_TEST(testAssignAndRetrieveGrades);
CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}

protected:

    // method to test the constructor
    void testConstructor();

    // method to test the assigning and retrieval of grades
    void testAssignAndRetrieveGrades();

};


#endif
