#ifndef MPLOTTER
#define MPLOTTER

#include "TGraph.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "solver.h"
#include <vector>
#include <string>
#include <fstream>

class MPlotter {
	public:
	MPlotter(const std::string& filename, double step = 10);
	~MPlotter();

	void run();

	private:
	void addPoint(const PosVec& X);
	bool isFileOk() const;
	void draw() const;
	void update() const;
	void findParameters();
	void initGraphs();
	

	std::vector<TGraph2D*> mgraphs2d;
	std::vector<TGraph*> mgraphs;
	std::vector<TCanvas*> mcanvases;
	TApplication* mapp;
	std::string mdrawOptions;
	std::string mfilename;
	unsigned int mstep;
	unsigned int mdim;
	unsigned int mnPoints;
	unsigned int mtotPoints;
	PosVec mins, maxs;
};

#endif //MPLOTTER
