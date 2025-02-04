CPUCC = g++

CXXFLAGS = -O3 

CC_LIBS = 

CC_SOURCES =  comparison.cpp lodepng.cpp
CC_OBJECTS = $(CC_SOURCES:%.cpp=%.o)

EXECNAME = comparison


all:
	$(CPUCC) -c $(CC_SOURCES) $(CXXFLAGS) $(CC_CXXFLAGS)
	$(CPUCC) -o $(EXECNAME) $(CC_LDFLAGS) $(CC_OBJECTS) $(CC_LIBS)


clean:
	rm -f *.o $(EXECNAME) *.linkinfo *~ *.bak .depend

