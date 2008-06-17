// CppUnit-Tutorial
// file: fractiontest.cc
#include "fractiontest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (fractiontest);

void fractiontest :: setUp (void)
{
	// set up test environment (initializing objects)
	a = new Fraction (1, 2);
	b = new Fraction (2, 3);
	c = new Fraction (2, 6);
	d = new Fraction (-5, 2);
	e = new Fraction (5, -2);
	f = new Fraction (-5, -2);
	g = new Fraction (5, 2);
	h = new Fraction ();
}

void fractiontest :: tearDown (void) 
{
	// finally delete objects
	delete a; delete b; delete c; delete d;
	delete e; delete f; delete g; delete h;
}

void fractiontest :: addTest (void)
{
	// check subtraction results
	CPPUNIT_ASSERT_EQUAL (*a + *b, Fraction (7, 6));
	CPPUNIT_ASSERT_EQUAL (*b + *c, Fraction (1));
	CPPUNIT_ASSERT_EQUAL (*d + *e, Fraction (-5));
	CPPUNIT_ASSERT_EQUAL (*e + *f, Fraction (0));
	CPPUNIT_ASSERT_EQUAL (*h + *c, Fraction (2, 6));
	CPPUNIT_ASSERT_EQUAL (*a + *b + *c + *d + *e + *f + *g + *h, Fraction (3, 2));
}

void fractiontest :: subTest (void)
{
	// check addition results
	CPPUNIT_ASSERT_EQUAL (*a - *b, Fraction (-1, 6));
	CPPUNIT_ASSERT_EQUAL (*b - *c, Fraction (1, 3));
	CPPUNIT_ASSERT_EQUAL (*b - *c, Fraction (2, 6));
	CPPUNIT_ASSERT_EQUAL (*d - *e, Fraction (0));
	CPPUNIT_ASSERT_EQUAL (*d - *e - *f - *g - *h, Fraction (-5));
}

void fractiontest :: exceptionTest (void)
{
	// an exception has to be thrown here
	CPPUNIT_ASSERT_THROW (Fraction (1, 0), DivisionByZeroException);
}

void fractiontest :: equalTest (void)
{
	// test successful, if true is returned
	CPPUNIT_ASSERT (*d == *e);
	CPPUNIT_ASSERT (Fraction (1) == Fraction (2, 2));
	CPPUNIT_ASSERT (Fraction (1) != Fraction (1, 2));
	// both must have equal valued
	CPPUNIT_ASSERT_EQUAL (*f, *g);
	CPPUNIT_ASSERT_EQUAL (*h, Fraction (0));
	CPPUNIT_ASSERT_EQUAL (*h, Fraction (0, 1));
}
