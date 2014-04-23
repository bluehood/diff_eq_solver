#include "solver.h"
#include <iostream>
#include <fstream>
#include <cmath>

//move along a geodesic on a "sine valley", that is, the graph in R^3 of the function z = sin(x) + sin(y); X=(x,y,x-dot,y-dot).

int main()
{
	PosVec X0 = { 0., 0., 1., .5 };
	ForceVec sinevalley_metric = {
		[] (double t, PosVec X) { return X[2]; },
		[] (double t, PosVec X) { return X[3]; },
		[] (double t, PosVec X) { return (2*cos(X[0])*(sin(X[0])*X[2]*X[2]+sin(X[1])*X[3]*X[3]))/(cos(2*X[0])+cos(2*X[1])+4); },
		[] (double t, PosVec X) { return (2*cos(X[1])*(sin(X[0])*X[2]*X[2]+sin(X[1])*X[3]*X[3]))/(cos(2*X[0])+cos(2*X[1])+4); },
	};

	RK4Solver rksolver(0.,X0,sinevalley_metric,0.01);
	
	std::ofstream rkOutput("rk4_sinevalley.txt");
	for(PosVec X = X0; rksolver.getTime() < 100; X = rksolver.step())
	{
		rkOutput << rksolver.getTime() << " ";
		rkOutput << X[0] << " " << X[1] << " " << sin(X[0]) + sin(X[1]);
		rkOutput << std::endl;
	}
	
	return 0;
}
