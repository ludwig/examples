CXX = g++
CXXFLAGS = -Wall
INCLUDE = -I/usr/include
LDFLAGS = -L/usr/lib
LIBS = -lcppunit

OBJS = \
	Course.o \
	Student.o \

TESTOBJS = \
	TestStudent.o \


all: tester

tester: $(OBJS) $(TESTOBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(TESTOBJS) tester

.PHONY: clean
