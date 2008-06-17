// CppUnit-Tutorial
// file: Fraction.h
#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>

using namespace std;

// some helper functions (actually don't belong here)
// calculates the greatest common factor (for reducing)
unsigned int gcf (unsigned int, unsigned int);
// calculates the least common denominator (for expanding)
unsigned int lcd (unsigned int, unsigned int);

// definition of an exception-class
class DivisionByZeroException
{
};

// simple definition of a fraction-class
class Fraction
{
	public:
		// constructor
		Fraction (int = 0, int = 1) throw (DivisionByZeroException);

		// copy-constructor and assignment-operator
		Fraction (const Fraction&);
		Fraction& operator= (const Fraction&);

		// comparing operators
		bool operator== (const Fraction&) const;
		bool operator!= (const Fraction&) const;

		// arithmetic operators
		friend Fraction operator+ (const Fraction&, const Fraction&);
		friend Fraction operator- (const Fraction&, const Fraction&);

		// output on stdout
		friend ostream& operator<< (ostream&, const Fraction&);

	private:
		// method for reducing
		void reduce (void);

		// variables for saving the numerator and denominator
		int numerator, denominator;
};

#endif
