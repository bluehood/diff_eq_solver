#include "maryplotter.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TAxis.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <sstream>

// FIXME cosa succede in run e findParameters se il numero di colonne cambia all'interno del file?
// TODO bisognerebbe usare sempre tante colonne quante ce ne sono nella prima riga o una cosa del genere
// TODO check reset. resetta tutto tutto??

MPlotter::MPlotter(const std::string& filename, double step) : 
	mdim(0), mstep(step), mdrawOptions("APLINE"),
	mfilename(filename), mnPoints(0), mtotPoints(0),
	mline(nullptr), mhist3d(nullptr) {	
	mapp = new TApplication("app",nullptr,nullptr);
}

MPlotter::~MPlotter() {
	delete mhist3d;
	delete mline;
	for(auto& mgraph : mgraphs)	
		if(mgraph != nullptr)
			delete mgraph;
	for(auto& mcanvas : mcanvases)
		if(mcanvas != nullptr)
			delete mcanvas;
	delete mapp;
}

void MPlotter::run() {
	if(mfilename.empty()){
		std::cerr  << "maryplotter: empty file nama. now quitting." << std::endl;
		return;
	}
	
	mfile.open(mfilename);
	//check whether filename is a good file
	if(!isFileOk()) {
		std::cerr << "maryplotter: run: could not open file \"" << mfilename << "\" or empty file found. now exiting." << std::endl;
		return;
	}
	
	findParameters();
	initGraphs();
	
	//parse file line by line
	//add a point and update canvases each time
	mfile.clear();
	mfile.seekg(0);
	std::string line;
	while(std::getline(mfile,line)) {
		std::stringstream str(line);
		PosVec X;
		double entry;
		while(str >> entry)
			X.push_back(entry);
		addPoint(X);
		draw();
		update();
		std::this_thread::sleep_for(std::chrono::milliseconds(mstep));
	}

	std::cout << "End of simulation. Press Enter to exit." << std::endl;
	std::cin.get();
}

bool MPlotter::isFileOk() {
	mfile.clear();
	mfile.seekg(0);
	if(!mfile){
		return false;
	}

	//check for empty file
	mfile.seekg(std::ios::ate);
	if(mfile.tellg() == 0){
		return false;
	}
	
	return true;
}

void MPlotter::addPoint(const PosVec& X) {
	if((mgraphs.empty() && mline == nullptr) || mcanvases.empty()) {
		std::cerr << "maryplotter: addPoint: empty graphs or canvases. this should never happen" << std::endl;
		throw 1;
	}
	
	if(X.size() != mdim) {
		std::cerr << "MPlotter: cannot add point. Point should be a vector of dimension " << mdim << std::endl;
		return;
	}

	if(mdim == 1) {
		mgraphs[0]->SetPoint(mnPoints,mnPoints,X[0]);
		++mnPoints;
	}
	else if(mdim == 4 || mdim == 2) {
		mgraphs[0]->SetPoint(mnPoints,X[0],X[1]);
		++mnPoints;
	}
	else if(mdim == 6 || mdim == 3) {
		mline->SetPoint(mnPoints,X[0],X[1],X[2]);
		++mnPoints;
	}
	else {
		std::cerr << "I have no idea how to handle a point with " << mdim << " coordinates, sorry" << std::endl;
		return;
	}
}

void MPlotter::draw() const {
	if(mdim == 1) {
		double miny = mins[0] - (mins[0]>0?0.1*mins[0]:-0.1*mins[0]);
		double maxy = maxs[0] + (maxs[0]>0?0.1*maxs[0]:-0.1*maxs[0]);
		mcanvases[0]->cd();
		mgraphs[0]->GetXaxis()->SetLimits(0.,1.1*mtotPoints);
		mgraphs[0]->GetXaxis()->SetRangeUser(0.,1.1*mtotPoints);
		mgraphs[0]->GetYaxis()->SetLimits(miny,maxy);
		mgraphs[0]->GetYaxis()->SetRangeUser(miny,maxy);
		mgraphs[0]->Draw(mdrawOptions.c_str());
	}
	else if(mdim == 4 || mdim == 2) {
		double minx = mins[0] - (mins[0]>0?0.1*mins[0]:-0.1*mins[0]);
		double maxx = maxs[0] + (maxs[0]>0?0.1*maxs[0]:-0.1*maxs[0]);
		double miny = mins[1] - (mins[1]>0?0.1*mins[1]:-0.1*mins[1]);
		double maxy = maxs[1] + (maxs[1]>0?0.1*maxs[1]:-0.1*maxs[1]);
		mcanvases[0]->cd();
		mgraphs[0]->GetXaxis()->SetLimits(minx,maxx);
		mgraphs[0]->GetXaxis()->SetRangeUser(minx,maxx);
		mgraphs[0]->GetYaxis()->SetLimits(miny,maxy);
		mgraphs[0]->GetYaxis()->SetRangeUser(miny,maxy);
		mgraphs[0]->Draw(mdrawOptions.c_str());
	}
	else if(mdim == 6 || mdim == 3) {
		double minx = mins[0] - (mins[0]>0?0.1*mins[0]:-0.1*mins[0]);
		double maxx = maxs[0] + (maxs[0]>0?0.1*maxs[0]:-0.1*maxs[0]);
		double miny = mins[1] - (mins[1]>0?0.1*mins[1]:-0.1*mins[1]);
		double maxy = maxs[1] + (maxs[1]>0?0.1*maxs[1]:-0.1*maxs[1]);
		double minz = mins[2] - (mins[2]>0?0.1*mins[2]:-0.1*mins[2]);
		double maxz = maxs[2] + (maxs[2]>0?0.1*maxs[2]:-0.1*maxs[2]);
		mcanvases[0]->cd();
		mhist3d->GetXaxis()->SetLimits(minx,maxx);
		mhist3d->GetXaxis()->SetRangeUser(minx,maxx);
		mhist3d->GetYaxis()->SetLimits(miny,maxy);
		mhist3d->GetYaxis()->SetRangeUser(miny,maxy);
		mhist3d->GetZaxis()->SetLimits(minz,maxz);
		mhist3d->GetZaxis()->SetRangeUser(minz,maxz);
		mhist3d->Draw();
		mline->Draw();
	}
	else {
		std::cerr << "I have no idea how to draw a system with dimension " << mdim << std::endl;
		return;
	}
}

void MPlotter::update() const {
	for(auto& mcanvas : mcanvases)
		if(mcanvas != nullptr) {
			mcanvas->Modified();
			mcanvas->Update();
		}
}

void MPlotter::findParameters() {
	mfile.clear();
	mfile.seekg(0);
	std::string line;
	
	//read a line
	while(std::getline(mfile,line)) {
		++mtotPoints;
		double entry;
		std::stringstream str(line);
		PosVec entries;
		while(str >> entry)
			entries.push_back(entry);
			
		if(mins.empty())
			mins = entries;
		if(maxs.empty())
			maxs = entries;
		for(int i=0; i<entries.size(); ++i) {
			if(entries[i] < mins[i])
				mins[i] = entries[i];
			if(entries[i] > maxs[i])
				maxs[i] = entries[i];
		}
	}
	
	mdim = mins.size();

	/* FOR DEBUG PURPOSES */
	std::cout << "parameters found:" << std::endl;
	std::cout << "dim = " << mdim << std::endl;
	std::cout << "n points = " << mtotPoints << std::endl;
	std::cout << "mins: ";
	for(auto x : mins)
		std::cout << x << " ";
	std::cout << std::endl;
	std::cout << "maxs: ";
	for(auto x : maxs)
		std::cout << x << " ";
	std::cout << std::endl;

	/**/
}

void MPlotter::initGraphs() {
	if(mins.empty() || maxs.empty()) {
		std::cerr << "maryplotter: initGraphs: min or max not present. this should never happen.";
		throw 1;
	}
	if(! (mgraphs.empty() && mline == nullptr && mcanvases.empty())) {
		std::cerr << "maryplotter: initGraphs: graphs or canvases already filled. this should never happen";
		throw 2;
	}
	
	if(mdim == 1) {
		mgraphs.push_back(new TGraph);
		mcanvases.push_back(new TCanvas);
	}
	else if(mdim == 4 || mdim == 2) {
		mgraphs.push_back(new TGraph);
		mcanvases.push_back(new TCanvas);
	}
	else if(mdim == 6 || mdim == 3) {
		mline = new TPolyLine3D;
		mhist3d = new TH3F("h3d", "", 3., -1., 1., 3., -1., 1., 3., -1., 1.);
		mhist3d->SetStats(false);
		mcanvases.push_back(new TCanvas);
	}
	else {
		std::cerr << "I have no idea how to handle a point with " << mdim << " coordinates, sorry" << std::endl;
		throw 3;
	}
}
