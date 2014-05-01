#include "solver.h" // PosVec, ForceVec, Solver, EulerSolver, RK2Solver, RK4Solver
#include <iostream>
#include <fstream>

int main()
{
	double a=10., b=28., c=8./3.;
	PosVec X0 = { -0.1, -0.01, .2};
	ForceVec lorenz_attractor = {
		[&] (double t, PosVec X) { return -a*X[0]+a*X[1]; },
		[&] (double t, PosVec X) { return X[0]*X[2]-X[1]+b*X[0]; },
		[&] (double t, PosVec X) { return -X[0]*X[1] - c*X[2]; }
	};

	RK4Solver rksolver(0.,X0,lorenz_attractor,0.001);
	
	std::ofstream output("lorenz.txt",std::ios::app);

	for(PosVec X = X0; rksolver.getTime() < 10; X = rksolver.step()){
		output << X[0] << " " << X[1] << " " << X[2]  << std::endl;
	}
	
	return 0;
}
