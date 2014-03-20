#include "solver.h"
#include <iostream>
#include <fstream>
#include <cmath>

int main()
{
	//initial conditions { x0, v0 }
	PosVec X0 = { 0., 0. };
	// duffing force { v, f(t,x,v) }
	ForceVec duffing_f = {
		[] (double t, PosVec X) { return X[1]; },
		[] (double t, PosVec X) { return X[0]*(1.-X[0]*X[0]) + cos(t) - X[1]; },
	};

	EulerSolver eulersolver(0.,X0,duffing_f,0.1);
	RK2Solver rk2solver(0.,X0,duffing_f,0.1);
	RK4Solver rk4solver(0.,X0,duffing_f,0.1);
	
	std::ofstream eulerOutput("euler_duffing.txt");
	std::ofstream rk2Output("rk2_duffing.txt");
	std::ofstream rk4Output("rk4_duffing.txt");
	for(PosVec X = X0; eulersolver.getTime() < 100; X = eulersolver.step())
	{
		eulerOutput << eulersolver.getTime() << " ";
		for(int i=0; i<3; ++i)
			eulerOutput << X[i] << " ";
		eulerOutput << std::endl;
	}

	for(PosVec X = X0; rk2solver.getTime() < 100; X = rk2solver.step())
	{
		rk2Output << rk2solver.getTime() << " ";
		for(int i=0; i<3; ++i)
			rk2Output << X[i] << " ";
		rk2Output << std::endl;
	}

	for(PosVec X = X0; rk4solver.getTime() < 100; X = rk4solver.step())
	{
		rk4Output << rk4solver.getTime() << " ";
		for(int i=0; i<3; ++i)
			rk4Output << X[i] << " ";
		rk4Output << std::endl;
	}
	
	return 0;
}
