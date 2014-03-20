#include "solver.h"
#include "exceptions.h"

Solver::Solver(double t0, const PosVec& X0, const ForceVec& F, double step) :
	mt(t0), mX(X0), mF(F), mstep(step) {
	checkDimensions();
}

PosVec Solver::step() {
	return step(mstep);
}
	
void Solver::reset(double t0, const PosVec& X0, const ForceVec& F, double step) {
	mt = t0;
	mX = X0;
	mF = F;
	mstep = step;
	checkDimensions();
}

void Solver::reset(double t0, const PosVec& X0, const ForceVec& F) {
	reset(t0,X0,F,mstep);
}

void Solver::reset(double t0, const PosVec& X0) {
	reset(t0,X0,mF,mstep);
}

void Solver::checkDimensions() const {
	if(mX.size()%2 != 0)
		throw wrongDimension();
	if(mX.size() != mF.size())
		throw dimensionMismatch();
}

PosVec EulerSolver::step(double step) {
	PosVec X(mX);
	for(int i=0; i<X.size(); ++i)
		X[i] += step*mF[i](mt,mX);
	mX = X;
	mt += step;
	return mX;
}

PosVec RK2Solver::step(double step) {
	PosVec X = mX;
	int dim = mX.size();

	std::vector<std::vector<double>> k(2);
	k[0] = k[1] = std::vector<double>(dim);
	
	std::vector<double> t_coef = { 0., 0.5 };
	std::vector<std::vector<double>> x_coefs(2);
	for(int i=0; i<x_coefs.size(); ++i)
		x_coefs[i] = std::vector<double>(i);
	x_coefs[1][0] = 0.5;
	std::vector<double> weights = { 0., 1. };

	for(int i=0; i<2; ++i)
		for(int j=0; j<dim; ++j) {
			PosVec midPoint = mX;
			for(int l=i-1; l>=0; --l)
				for(int m=0; m<dim; ++m)
					midPoint[m] += x_coefs[i][l]*k[l][m];
			k[i][j] = step*mF[j](mt + t_coef[i]*step, midPoint);
		}
	
	//evaluate new position
	for(int i=0; i<dim; ++i)
		X[i] += weights[0]*k[0][i] + weights[1]*k[1][i];
	mX = X;
	mt += step;
	return mX;
}

PosVec RK4Solver::step(double step) {
	PosVec X = mX;
	int dim = mX.size();

	std::vector<std::vector<double>> k(4);
	for(int i=0; i<4; ++i)
		k[i] = std::vector<double>(dim);
	
	// we are using kutta's coefficients for RK4
	std::vector<double> t_coef = { 0., 1./3., 2./3., 1.};
	std::vector<std::vector<double>> x_coefs(4);
	for(int i=0; i<x_coefs.size(); ++i)
		x_coefs[i] = std::vector<double>(i);
	x_coefs[1][0] = 1./3.;
	x_coefs[2][1] = 1.;
	x_coefs[2][0] = -1./3.;
	x_coefs[3][2] = 1.;
	x_coefs[3][1] = -1.;
	x_coefs[3][0] = 1.;
	std::vector<double> weights = { 1./8., 3./8., 3./8., 1./8. };

	for(int i=0; i<4; ++i)
		for(int j=0; j<dim; ++j) {
			PosVec midPoint = mX;
			for(int l=i-1; l>=0; --l)
				for(int m=0; m<dim; ++m)
					midPoint[m] += x_coefs[i][l]*k[l][m];
			k[i][j] = step*mF[j](mt + t_coef[i]*step, midPoint);
		}
	
	//evaluate new position
	for(int i=0; i<4; ++i)
		for(int j=0; j<dim; ++j)
			X[j] += weights[i]*k[i][j];
	mX = X;
	mt += step;
	return mX;
}
