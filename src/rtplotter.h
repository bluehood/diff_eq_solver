#ifndef RTPLOTTER
#define RTPLOTTER

#include "TGraph.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "solver.h"
#include <vector>
#include <string>

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
	std::vector<TGraph2D*> mgraphs2d;
	std::vector<TGraph*> mgraphs;
	std::vector<TCanvas*> mcanvases;
	TApplication* app;
	unsigned int mdim;
	unsigned int mnPoints;
	char mmode;
	bool isDimSet;
	std::string mdrawOptions;
};

#endif //RTPLOTTER
