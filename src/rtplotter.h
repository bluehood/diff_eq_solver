#ifndef RTPLOTTER
#define RTPLOTTER

#include "TGraph.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TH3F.h"
#include "TPolyLine3D.h"
#include "solver.h"
#include <vector>
#include <string>
#include <cmath>

class RTPlotter {
	public:
	RTPlotter(char mode = 't');
	~RTPlotter();

	void addPoint(const PosVec& X);
	void draw(const std::string& opt);
	void draw() const;
	void update() const;
	void reset();

	unsigned int getNPoints() const { return mnPoints; }

	private:
	std::vector<TGraph*> mgraphs;
	std::vector<TCanvas*> mcanvases;
	TPolyLine3D* mline;
	TH3F* mhist3d;
	TApplication* app;
	unsigned int mdim;
	unsigned int mnPoints;
	char mmode;
	bool isDimSet;
	std::string mdrawOptions;
	std::vector<double> mins,maxs;

	void evalMinMax(double x, double y = HUGE_VAL, double z = HUGE_VAL);
};

#endif //RTPLOTTER
