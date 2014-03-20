#include "solver.h" //PosVec, ForceVec, (virtual) Solver, EulerSolver, RK2Solver, RK4Solver
#include <iostream>

int main() {
	// define a vector containing initial conditions
	PosVec X0 = { 0., 0. };
	// define force vector: this is a vector of std::function<double(double,ForceVec)>
	ForceVec F = {
		[] (double t, PosVec X) { return X[1]; },
		[] (double t, PosVec X) { return 1.; }
	};

	// define solver(t0, X0, F, step)
	RK4Solver solver(0., X0, F, 0.1);
	
	// solve step by step
	for(int i=0; i<100; ++i) {
		PosVec X = solver.step();
		double t = solver.getTime();
		std::cout << t << " " << X[0] << " " << X[1] << std::endl;
	}

	return 0;
}
