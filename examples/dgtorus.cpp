#include "solver.h"
#include <iostream>
#include <fstream>
#include <cmath>

//a particle moves on a torus subject to gravity and linear damping; coordinate vector is X=(theta,phi,theta-dot,phi-dot) (theta detects the section and phi the position at that section; both are only specified mod 2*M_PI).

int main()
{
	double R = 1., r = .5; // outer/inner radii; any pair 0 < r < R will do
	double g = 1., gamma = .03; // g = gravity, gamma = damping
	PosVec X0 = { 0., 0., 1., 1. }; // initial conditions
	ForceVec tot_force = { 
		[&] (double t, PosVec X) { return X[2]; },
		[&] (double t, PosVec X) { return X[3]; },
		[&] (double t, PosVec X) { return /*geodesic motion*/ -2*r*cos(X[1])*X[2]*X[3]/(R+r*sin(X[1])) - /*damping*/ gamma*X[2]; },
		[&] (double t, PosVec X) { return /*geodesic motion*/ (R+r*sin(X[1]))*cos(X[1])*X[2]*X[2]/r + /*gravity*/ g*sin(X[1])/r - /*damping*/ gamma*2*X[3]; },
	};

	RK4Solver rksolver(0.,X0,tot_force,0.01);
	
	std::ofstream rkOutput("rk4_dgtorus_diag.txt");
	for(PosVec X = X0; rksolver.getTime() < 200; X = rksolver.step())
	{
		rkOutput << rksolver.getTime() << " ";
		rkOutput << (1+.5*sin(X[1]))*cos(X[0]) << " " << (1+.5*sin(X[1]))*sin(X[0]) << " " << .5*cos(X[1]); // embedding the trajectory in R^3
		rkOutput << std::endl;
	}
	
	return 0;
}
