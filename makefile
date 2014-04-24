CXX := g++
CXX_FLAGS := -std=c++11 #-ggdb
SOLVER_INCS := -Isrc
SOLVER_LIBS := -Llib -lsolver -lplotter
ROOT_INCS := `root-config --cflags`
ROOT_LIBS := `root-config --libs`

executables := examples/spyral_motion.x examples/example.x \
examples/duffing.x examples/simple_example.x \
examples/rtplotter_example.x examples/maryplotter_example.x \
examples/dgtorus.x examples/gtorus.x examples/duffing.x \
examples/sinevalley.x
libraries := lib/libsolver.a lib/libplotter.a

all: lib ${libraries} ${executables}
all_noroot: lib lib/libsolver.a examples/duffing.x \
examples/example.x examples/simple_example.x \
examples/spyral_motion.x

%.x: %.cpp lib/libsolver.a
	${CXX} ${CXX_FLAGS} -o $@ $< -Isrc -Llib -lsolver

examples/rtplotter_example.x: examples/rtplotter_example.cpp \
${libraries}
	${CXX} ${CXX_FLAGS} -o $@ $< ${SOLVER_INCS} ${SOLVER_LIBS} ${ROOT_INCS} ${ROOT_LIBS}

examples/maryplotter_example.x: examples/maryplotter_example.cpp \
${libraries}
	${CXX} ${CXX_FLAGS} -o $@ $< ${SOLVER_INCS} ${SOLVER_LIBS} ${ROOT_INCS} ${ROOT_LIBS}

lib/libsolver.a: lib/solver.o lib/exceptions.o
	ar -cvr $@ $^

lib/libplotter.a: lib/rtplotter.o lib/maryplotter.o
	ar -cvr $@ $^

lib/solver.o: src/solver.cpp src/solver.h src/exceptions.h
	${CXX} ${CXX_FLAGS} -c -o $@ $<

lib/rtplotter.o: src/rtplotter.cpp src/rtplotter.h
	${CXX} ${CXX_FLAGS} -c -o $@ $< ${ROOT_LIBS} ${ROOT_INCS}

lib/maryplotter.o: src/maryplotter.cpp src/maryplotter.h
	${CXX} ${CXX_FLAGS} -c -o $@ $< ${ROOT_LIBS} ${ROOT_INCS}

lib/exceptions.o: src/exceptions.cpp src/exceptions.h
	${CXX} ${CXX_FLAGS} -c -o $@ $<

.PHONY: clean
clean:
	rm -rf ${executables} lib

lib:
	mkdir lib
