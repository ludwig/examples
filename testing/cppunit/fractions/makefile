# CppUnit-Tutorial
# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

ftest: ftest.o fractiontest.o Fraction.o
	gcc -o ftest ftest.o fractiontest.o Fraction.o -L${CPPUNIT_PATH}/lib -lstdc++ -lcppunit -ldl

Fraction.o: Fraction.cc Fraction.h
	gcc -c Fraction.cc

fractiontest.o: fractiontest.cc
	gcc -c fractiontest.cc -I${CPPUNIT_PATH}/include

ftest.o: ftest.cc
	gcc -c ftest.cc -I${CPPUNIT_PATH}/include

clean:
	rm -f *.o ftest
