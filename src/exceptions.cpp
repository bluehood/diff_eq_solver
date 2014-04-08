#include "exceptions.h"

const char* wrongDimension::what() const throw() {
	return "Solver: a PosVec must have an even number of components!";
}	

const char* emptyPosVec::what() const throw() {
	return "Solver: a PosVec must have at least one component!";
}

const char* dimensionMismatch::what() const throw() {
	return "Solver: position vector and force vector must have the same size!";
}	

const char* badFile::what() const throw() {
	return "MPlotter: could not read file";
}	
