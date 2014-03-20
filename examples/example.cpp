#include "solver.h" // PosVec, ForceVec, Solver, EulerSolver, RK2Solver, RK4Solver
#include <iostream>
#include <fstream>

int main()
{
	// define initial conditions vector { x1, x2, ..., v1, v2, ...}
	PosVec X0 = { 0., 1. };
	// define force vector. this is a vector of std::function<double(double,PosVec)>
	// the differential equation that will be solved is dX/dt = F(t,X(t))
	// In our example F = { v, -x } (simple harmonic oscillator with frequency=1)
	ForceVec double_osc_f = {
		[] (double t, PosVec X) { return X[1]; },
		[] (double t, PosVec X) { return -X[0]; }
	};

	// Solvers can be declared as instance of a derived class...
	// Solver(t0, X0, Force, step)
	EulerSolver eulersolver(0.,X0,double_osc_f,0.1);
	// ...or as pointers to the base class
	Solver* rksolver = new RK2Solver(0.,X0,double_osc_f,0.1);
	
	// we want all output printed on file
	std::ofstream eulerOutput("euler_example.txt");
	std::ofstream rkOutput("rk2_example.txt");

	// do 100 steps
	for(int i=0; i<100; ++i)
	{
		PosVec X = eulersolver.step();
		double t = eulersolver.getTime();
		// print current time
		eulerOutput << t << " ";
		// print coordinates
		for(int i=0; i<2; ++i)
			eulerOutput << X[i] << " ";
		eulerOutput << std::endl;
	}

	// same for the Runge-Kutta solver
	/* we use a compact (although not very readable) way to do enough steps
		to arrive to the desired time */
	// we use a different timestap than the one specified in the constructor
	for(PosVec X = X0; rksolver->getTime() < 100; X = rksolver->step(0.2))
		rkOutput << rksolver->getTime() << " " << X[0] << " " << X[1] << std::endl;
	
	return 0;
}
