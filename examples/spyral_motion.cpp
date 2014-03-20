#include "solver.h"
#include <iostream>
#include <fstream>
#include <cmath>

int main()
{
	PosVec X0 = { 1./sqrt(2.), 1./sqrt(2.), 0., 0., 0., 1. };
	ForceVec double_osc_f = {
		[] (double t, PosVec X) { return X[3]; },
		[] (double t, PosVec X) { return X[4]; },
		[] (double t, PosVec X) { return X[5]; },
		[] (double t, PosVec X) { return -0.7*X[0]; },
		[] (double t, PosVec X) { return -X[1]; },
		[] (double t, PosVec X) { return 0; }
	};

	EulerSolver eulersolver(0.,X0,double_osc_f,0.1);
	RK2Solver rksolver(0.,X0,double_osc_f,0.1);
	
	std::ofstream eulerOutput("euler_spyral.txt");
	std::ofstream rkOutput("rk2_spyral.txt");
	for(PosVec X = X0; eulersolver.getTime() < 100; X = eulersolver.step())
	{
		eulerOutput << eulersolver.getTime() << " ";
		for(int i=0; i<3; ++i)
			eulerOutput << X[i] << " ";
		eulerOutput << std::endl;
	}
	for(PosVec X = X0; rksolver.getTime() < 100; X = rksolver.step())
	{
		rkOutput << rksolver.getTime() << " ";
		for(int i=0; i<3; ++i)
			rkOutput << X[i] << " ";
		rkOutput << std::endl;
	}
	
	return 0;
}
