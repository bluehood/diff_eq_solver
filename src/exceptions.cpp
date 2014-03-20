#include "exceptions.h"

const char* wrongDimension::what() const throw() {
	return "Solver: a PosVec must have an even number of components!";
}	

const char* dimensionMismatch::what() const throw() {
	return "Solver: position vector and force vector must have the same size!";
}	
