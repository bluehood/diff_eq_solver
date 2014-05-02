#include "rtplotter.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

RTPlotter::RTPlotter(char mode) : 
	mdim(0), isDimSet(false), mmode(mode), mnPoints(0), 
	mdrawOptions("APLINE"), mhist3d(nullptr), mline(nullptr){
	app = new TApplication("app",nullptr,nullptr);
}

RTPlotter::~RTPlotter() {
	//reset does a bit more than just cleaning up, but code is cleaner this way
	reset();
}

void RTPlotter::addPoint(const PosVec& X) {
	if(!isDimSet) {
		mdim = X.size();
		isDimSet = true;
		std::cout << "phase space dim = " << mdim << std::endl;
		std::cout << "draw mode = " << mmode << std::endl;
	}	
	if(X.size() != mdim) {
		std::cerr << "RTPlotter: cannot add point. Point should be a vector of dimension " << mdim << std::endl;
		return;
	}
	
	if(mdim == 2) {
		double x,y;
		if(mmode == 't') {
			x = mnPoints;
			y = X[0];
		}
		else if(mmode == 'p') {
			x = X[0];
			y = X[1];
		}
		if(mgraphs.size() == 0)
			mgraphs.push_back(new TGraph);
		if(mcanvases.size() == 0)
			mcanvases.push_back(new TCanvas);
		mgraphs[0]->SetPoint(mnPoints,x,y);
		++mnPoints;
	}
	else if( mdim == 4) {
		if(mmode == 't') {
			double x,y;
			x = X[0];
			y = X[1];
			if(mgraphs.size() == 0)
				mgraphs.push_back(new TGraph);
			if(mcanvases.size() == 0)
				mcanvases.push_back(new TCanvas);
			mgraphs[0]->SetPoint(mnPoints,x,y);
			++mnPoints;
		}
		else if(mmode == 'p') {
			double x1,v1,x2,v2;
			x1 = X[0];
			x2 = X[1];
			v1 = X[2];
			v2 = X[3];
			if(mgraphs.size() == 0) {
				mgraphs.push_back(new TGraph);
				mgraphs.push_back(new TGraph);
			}
			if(mcanvases.size() == 0) {
				mcanvases.push_back(new TCanvas);
				mcanvases.push_back(new TCanvas);
			}
			mgraphs[0]->SetPoint(mnPoints,x1,v1);
			mgraphs[1]->SetPoint(mnPoints,x2,v2);
			++mnPoints;
		}
	}
	else if(mdim == 6) {
		if(mmode == 't') {
			double x,y,z;
			x = X[0];
			y = X[1];
			z = X[2];
			evalMinMax(x,y,z);
			if(mcanvases.size() == 0)
				mcanvases.push_back(new TCanvas);
			if(mhist3d == nullptr)
				mhist3d = new TH3F("h3d", "", 3., -1., 1., 3., -1., 1., 3., -1., 1.);
			if(mline == nullptr)
				mline = new TPolyLine3D;
			mline->SetPoint(mnPoints,x,y,z);
			++mnPoints;
		}
		else if(mmode == 'p') {
			double x1,v1,x2,v2,x3,v3;
			x1 = X[0];
			x2 = X[1];
			x3 = X[2];
			v1 = X[3];
			v2 = X[4];
			v3 = X[5];
			if(mgraphs.size() == 0) {
				mgraphs.push_back(new TGraph);
				mgraphs.push_back(new TGraph);
				mgraphs.push_back(new TGraph);
			}
			if(mcanvases.size() == 0) {
				mcanvases.push_back(new TCanvas);
				mcanvases.push_back(new TCanvas);
				mcanvases.push_back(new TCanvas);
			}
			mgraphs[0]->SetPoint(mnPoints,x1,v1);
			mgraphs[1]->SetPoint(mnPoints,x2,v2);
			mgraphs[2]->SetPoint(mnPoints,x3,v3);
			++mnPoints;
		}
	}
	else {
		std::cerr << "RTPlotter: I have no idea how to add a point with " << mdim << " coordinates, sorry" << std::endl;
		return;
	}
}

void RTPlotter::draw(const std::string& opt) {
	mdrawOptions = opt;
	draw();
}

void RTPlotter::draw() const {
	if(mdim == 2) {
		if(mmode == 't') {
			mcanvases[0]->cd();
			mgraphs[0]->Draw(mdrawOptions.c_str());
		}
		else if(mmode == 'p') {
			mcanvases[0]->cd();
			mgraphs[0]->Draw(mdrawOptions.c_str());
		}
	}
	else if(mdim == 4) {
		if(mmode == 't') {
			mcanvases[0]->cd();
			mgraphs[0]->Draw(mdrawOptions.c_str());
		}
		else if(mmode == 'p') {
			for(int i=0; i<2; ++i) {
				mcanvases[i]->cd();
				mgraphs[i]->Draw(mdrawOptions.c_str());
			}
		}
	}
	else if(mdim == 6) {
		if(mmode == 't') {	
			mcanvases[0]->cd();
			if(mins[0] < maxs[0])
				mhist3d->GetXaxis()->SetLimits(mins[0],maxs[0]);
			if(mins[1] < maxs[1])
				mhist3d->GetYaxis()->SetLimits(mins[1],maxs[1]);
			if(mins[2] < maxs[2])
				mhist3d->GetZaxis()->SetLimits(mins[2],maxs[2]);
			mhist3d->SetStats(false);
			mhist3d->Draw();
			mline->Draw();
		}
		else if(mmode == 'p') {
			for(int i=0; i<3; ++i) {
				mcanvases[i]->cd();
				mgraphs[i]->Draw(mdrawOptions.c_str());
			}
		}
	}
	else {
		std::cerr << "RTPlotter: I have no idea how to draw a system with dimension " << mdim << std::endl;
		return;
	}
}

void RTPlotter::update() const {
	if(mhist3d != nullptr){
		if(mins[0] < maxs[0])
			mhist3d->GetXaxis()->SetLimits(mins[0],maxs[0]);
		if(mins[1] < maxs[1])
			mhist3d->GetYaxis()->SetLimits(mins[1],maxs[1]);
		if(mins[2] < maxs[2])
			mhist3d->GetZaxis()->SetLimits(mins[2],maxs[2]);
	}
	for(auto& mcanvas : mcanvases)
		if(mcanvas != nullptr) {
			mcanvas->Modified();
			mcanvas->Update();
		}
}

void RTPlotter::reset() {
	mnPoints = 0;
	isDimSet = false;
	mdim = 0;
	mmode = 't';
	mdrawOptions = "APLINE";
	delete mhist3d;
	mhist3d = nullptr;
	delete mline;
	mline = nullptr;
	for(auto& mgraph : mgraphs)	
		if(mgraph != nullptr)
			delete mgraph;
	for(auto& mcanvas : mcanvases)
		if(mcanvas != nullptr)
			delete mcanvas;
	mgraphs.clear();
	mcanvases.clear();
}

void RTPlotter::evalMinMax(double x, double y, double z) {
	if(mins.size() < 1)
		mins.push_back(HUGE_VAL);
	if(maxs.size() < 1)
		maxs.push_back(-HUGE_VAL);
	if(mins[0] > x)
		mins[0] = x;
	if(maxs[0] < x)
		maxs[0] = x;

	if(y != HUGE_VAL){
		if(mins.size() < 2)
			mins.push_back(HUGE_VAL);
		if(maxs.size() < 2)
			maxs.push_back(-HUGE_VAL);
		if(mins[1] > y)
			mins[1] = y;
		if(maxs[1] < y)
			maxs[1] = y;
	}

	if(z != HUGE_VAL){
		if(mins.size() < 3)
			mins.push_back(HUGE_VAL);
		if(maxs.size() < 3)
			maxs.push_back(-HUGE_VAL);
		if(mins[2] > z)
			mins[2] = z;
		if(maxs[2] < z)
			maxs[2] = z;
	}
}
