#include "solver.h"
#include <iostream>
#include <fstream>
#include <cmath>

//move along a geodesic on a torus; to do this, just set the initial position and velocity, with X=(theta,phi,theta-dot,phi-dot) (theta is the "big" angle of the torus while phi is the "small" one, if that makes any sense to you). The initial vector is parallel transported along a geodesic whose starting tangent vector is the specified velocity.
//WITH GRAVITY
int main()
{
	double R = 1., r = .5, g = 10;
	PosVec X0 = { 0., -M_PI/2., 1., 1. }; //moving along the "vertical" originating circle
	ForceVec gtorus_metric = { // here we are using R=1, r=.5 (any pair 0<r<R will do)
		[&] (double t, PosVec X) { return X[2]; },
		[&] (double t, PosVec X) { return X[3]; },
		[&] (double t, PosVec X) { return -2*r*cos(X[1])*X[2]*X[3]/(R+r*sin(X[1])); },
		[&] (double t, PosVec X) { return (R+r*sin(X[1]))*cos(X[1])*X[2]*X[2]/r + g*sin(X[1])/r; },
	};

	RK4Solver rksolver(0.,X0,gtorus_metric,0.01);
	
	std::ofstream rkOutput("rk4_gtorus_diag.txt");
	for(PosVec X = X0; rksolver.getTime() < 100; X = rksolver.step())
	{
		rkOutput << rksolver.getTime() << " ";
		rkOutput << (1+.5*sin(X[1]))*cos(X[0]) << " " << (1+.5*sin(X[1]))*sin(X[0]) << " " << .5*cos(X[1]);
		rkOutput << std::endl;
	}
	
	return 0;
}
