CXX := g++
CXX_FLAGS := -std=c++11 #-ggdb
SOLVER_INCS := -Isrc
SOLVER_LIBS := -Llib -lsolver

executables := examples/spyral_motion.x examples/example.x examples/duffing.x examples/simple_example.x

all: lib lib/libsolver.a ${executables}
	rm src/*.o

%.x: %.cpp
	${CXX} ${CXX_FLAGS} -o $@ $< ${SOLVER_LIBS} ${SOLVER_INCS}

lib/libsolver.a: src/solver.o src/exceptions.o
	ar -cvr $@ $^

src/solver.o: src/solver.cpp src/solver.h src/exceptions.h
	${CXX} ${CXX_FLAGS} -c -o $@ $<

src/exceptions.o: src/exceptions.cpp src/exceptions.h
	${CXX} ${CXX_FLAGS} -c -o $@ $<

.PHONY: clean
clean:
	rm -rf ${executables} src/*.o lib

lib:
	mkdir lib
