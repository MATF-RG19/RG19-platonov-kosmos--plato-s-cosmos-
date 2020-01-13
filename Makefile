PROGRAM = main
CXX      = g++
CXXFLAGS  = -g -Wall -Wextra -std=c++17
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o classes.o glm/libglm_static.a
	$(CXX) $^ -o $@ $(LDLIBS) $(CXXFLAGS)

main.o: main.cpp
	$(CXX) -c $< $(CXXFLAGS)

classes.o: classes.cpp
	$(CXX) -c $< $(CXXFLAGS)

.PHONY: clean

clean:
	-rm *.o $(PROGRAM) *core
