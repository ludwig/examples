// CppUnit-Tutorial
// file: fractiontest.h
#ifndef FRACTIONTEST_H
#define FRACTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Fraction.h"

using namespace std;

class fractiontest : public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (fractiontest);
	CPPUNIT_TEST (addTest);
	CPPUNIT_TEST (subTest);
	CPPUNIT_TEST (exceptionTest);
	CPPUNIT_TEST (equalTest);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void addTest (void);
		void subTest (void);
		void exceptionTest (void);
		void equalTest (void);

	private:
		Fraction *a, *b, *c, *d, *e, *f, *g, *h;
};

#endif
