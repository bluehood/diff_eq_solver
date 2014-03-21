#include "solver.h"
#include "plotter.h"
#include "TApplication.h"

int main() {
	MPlotter plotter("trajectory.txt");
	plotter.run();
	return 0;
}
