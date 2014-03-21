#include "rtplotter.h"
#include <iostream>
#include <vector>
#include <string>

RTPlotter::RTPlotter(char mode) : 
	mdim(0), isDimSet(false), mmode(mode), mnPoints(0), mdrawOptions("APLINE") {	
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
	else if( mdim == 6) {
		if(mmode == 't') {
			double x,y,z;
			x = X[0];
			y = X[1];
			z = X[2];
			if(mgraphs2d.size() == 0)
				mgraphs2d.push_back(new TGraph2D);
			if(mcanvases.size() == 0)
				mcanvases.push_back(new TCanvas);
			mgraphs2d[0]->SetPoint(mnPoints,x,y,z);
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
		std::cerr << "I have no idea how to handle a point with " << mdim << " coordinates, sorry" << std::endl;
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
			mgraphs2d[0]->Draw(mdrawOptions.c_str());
		}
		else if(mmode == 'p') {
			for(int i=0; i<3; ++i) {
				mcanvases[i]->cd();
				mgraphs[i]->Draw(mdrawOptions.c_str());
			}
		}
	}
	else {
		std::cerr << "I have no idea how to draw a system with dimension " << mdim << std::endl;
		return;
	}
	//app->Run();
}

void RTPlotter::update() const {
	for(auto& mgraph2d : mgraphs2d)
		if(mgraph2d != nullptr)
			mgraph2d->Draw(mdrawOptions.c_str());
	for(auto& mgraph : mgraphs)	
		if(mgraph != nullptr)
			mgraph->Draw(mdrawOptions.c_str());
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
	for(auto& mgraph2d : mgraphs2d)
		if(mgraph2d != nullptr)
			delete mgraph2d;
	for(auto& mgraph : mgraphs)	
		if(mgraph != nullptr)
			delete mgraph;
	for(auto& mcanvas : mcanvases)
		if(mcanvas != nullptr)
			delete mcanvas;
	mgraphs2d.clear();
	mgraphs.clear();
	mcanvases.clear();
}
