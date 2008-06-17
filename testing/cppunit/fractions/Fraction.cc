// CppUnit-Tutorial
// file: fraction.cc
#include "Fraction.h" 

unsigned int gcf (unsigned int n1, unsigned int n2)
{
	unsigned int TMP;

	if (n1 < n2)
	{
		TMP = n1;
		n1 = n2;
		n2 = TMP;
	}
	if (n2 > 0)
		return (gcf (n2, n1 % n2));
	else
		return (n1);
}

unsigned int lcd (unsigned int n1, unsigned int n2)
{
	return n1 * n2 / gcf (n1, n2);
}

Fraction :: Fraction (int n, int d) throw (DivisionByZeroException)
{
	// throw exception if denominator 0
	if (d == 0)
		throw DivisionByZeroException ();
	if (d < 0)
	{
		numerator = -n;
		denominator = -d;
	}
	else
	{
		numerator = n;
		denominator = d;
	}
	reduce ();
}

Fraction :: Fraction (const Fraction& v)
{
	numerator = v.numerator;
	denominator = v.denominator;
}

Fraction& Fraction :: operator= (const Fraction& v)
{
	numerator = v.numerator;
	denominator = v.denominator;
	return *this;
}

bool Fraction :: operator== (const Fraction& v) const
{
	return numerator == v.numerator && denominator == v.denominator;
}

bool Fraction :: operator!= (const Fraction& v) const
{
	return !operator== (v);
}

void Fraction :: reduce (void)
{
	unsigned int teiler = gcf (abs (numerator), abs (denominator));

	numerator /= (int) teiler;
	denominator /= (int) teiler;
}

Fraction operator+ (const Fraction& a, const Fraction& b)
{
	unsigned int v = lcd (abs (a.denominator), abs (b.denominator));

	Fraction r (a.numerator * ((int) v / a.denominator) + b.numerator * ((int) v / b.denominator), v);
	r.reduce ();
	return r;
}

Fraction operator- (const Fraction& a, const Fraction& b)
{
	unsigned int v = lcd (abs (a.denominator), abs (b.denominator));

	Fraction r (a.numerator * ((int) v / a.denominator) - b.numerator * ((int) v / b.denominator), v);
	r.reduce ();
	return r;
}

ostream& operator<< (ostream& out, const Fraction& v)
{
	return out << v.numerator << "/" << v.denominator;
}
