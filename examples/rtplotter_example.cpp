#include "solver.h"
#include "plotter.h"
#include "TApplication.h"
#include <iostream>
#include <thread>

int main() {
	// define a vector containing initial conditions
	PosVec X0 = { 1., 0., 0., 0., 1., 1. };
	// define force vector: this is a vector of std::function<double(double,ForceVec)>
	ForceVec F = {
		[] (double t, PosVec X) { return X[3]; },
		[] (double t, PosVec X) { return X[4]; },
		[] (double t, PosVec X) { return X[5]; },
		[] (double t, PosVec X) { return -X[0]; },
		[] (double t, PosVec X) { return -X[1]; },
		[] (double t, PosVec X) { return 0.; }
	};

	// define solver(t0, X0, F, step)
	RK4Solver solver(0., X0, F, 0.1);
	
	// define RTPlotter ('t' for trajectory, 'p' for phase space)
	RTPlotter RTPlotter('t');

	// solve step by step
	//for(int i=0; i<100; ++i) {
	while(1){
		PosVec X = solver.step();
		RTPlotter.addPoint(X);
		RTPlotter.draw();
		RTPlotter.update();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
