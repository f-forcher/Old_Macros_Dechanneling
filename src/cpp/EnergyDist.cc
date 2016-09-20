//... include header files ...

#include "../EnergyDist.h"

#include <TDirectory.h>
#include <TFile.h>
#include <TH1.h>
#include <iostream>
#include <fstream>

#include "../AnalysisFactory.h"
#include "../AnalysisInfo.h"
#include "../BraggStatistic.h"
#include "../dbg_macro.h"
#include "../TotalEnergy.h"
#include "../util/include/Singleton.h"
#include "../dbg_macro.h"


using namespace std;

// builder to create an EnergyDist analyzer
class EnergyDistBuilder: public AnalysisFactory::Builder {
public:
	// assign "hist" as name for this analyzer and builder
	EnergyDistBuilder() :
			AnalysisFactory::Builder("hist") {
	}
	// create an EnergyDist when builder is run
	virtual AnalysisSteering* build(const AnalysisInfo* info) {
		return new EnergyDist(info);
	}
};
// create a global EnergyDistBuilder, so that it is created and registered
// before main execution start:
// when the AnalysisFactory::create function is run,
// an EnergyDistBuilder will be available with name "hist".
static EnergyDistBuilder edist;

EnergyDist::EnergyDist(const AnalysisInfo* info) :
		AnalysisSteering(info) {
	histname = string(info->value("hist"));
	totenergy_graph =  new TH1F("tot_energy", "Total Energy", 100, 0, 10000);
}

EnergyDist::~EnergyDist() {
	for (auto& bc : vbraggCurv) {
		delete bc->bragg_statistic;
		delete bc->braggStat_graph;
		delete bc;
	}
}

// function to be called at execution start
void EnergyDist::beginJob() {

// create energy distributions for different total energy ranges

	std::ifstream ifs(aInfo->value("ranges"));
	DBG(clog << aInfo->value("ranges") << endl; , ;);

	if (!ifs) {
		cerr << "[ERROR]: file not opened";
		throw -2;
	}

	int id;
	int min_energy;
	int max_energy;

	while (ifs >> id) {
		ifs >> min_energy;
		ifs >> max_energy;

		bCreate(id, min_energy, max_energy);
	}

	return;
}

// function to be called at execution end
void EnergyDist::endJob() {

//... open ROOT file ...
	TDirectory* currentDir = gDirectory;
	const char* filename = (histname + string(".root")).c_str();
	TFile* file = new TFile(filename, "RECREATE");

// fill distributions with mean and rms energies

// number of points
	auto NPOINTS = Event::minSize();

// loop over distributions
	for (auto& bc : vbraggCurv) {
		// get Bragg curve informations:

		// mean and rms energies and energy loss graph

		// compute results
		bc->bragg_statistic->compute();

		// get mean and rms energies
		//unsigned NPOINTS = bc->bragg_statistic->eMean().size();
		const auto& mean = bc->bragg_statistic->eMean();
		const auto& rms = bc->bragg_statistic->eRMS();

		// loop over points
		for (unsigned i = 1; i <= NPOINTS; ++i) {
			// set center and error values in the graph
			// by using SetBinContent and SetBinError, bin count starts from 1
			bc->braggStat_graph->SetBinContent(i, mean[i - 1]);
			bc->braggStat_graph->SetBinError(i, rms[i - 1]);
			DBG(cerr << i << ": " << mean[i - 1] << " +/- " << rms[i - 1]
						<< endl
			;
				, ;)
		}
		// save distribution
		bc->braggStat_graph->Write();
	}

	//Save also totenergy_graph
	totenergy_graph->Write();


//... close ROOT file ...
	delete file;
	currentDir->cd();

	return;

}

// function to be called for each event
void EnergyDist::update(const Event& ev) {
	//Make total energy graph
	totenergy_graph->Fill(TotalEnergy::instance()->energy());

// loop over energy distributions and pass event to each of them
	for (auto& bcurve : vbraggCurv) {



		bcurve->bragg_statistic->add(ev);


	}
	return;
}

// create objects for a Bragg curve
void EnergyDist::bCreate(int id, float min, float max) {

// create energy distribution for events with total energy in given range

// create name for TH1F object as "hMeaX" with X given by id
	string name = string("hMea") + to_string(id);

// bin number equal to point number
	const unsigned NBIN = Event::minSize();

// create TH1F and statistic objects and store their pointers
	BraggCurve* bc = new BraggCurve;
	bc->bragg_statistic = new BraggStatistic(min, max);
	bc->braggStat_graph = new TH1F(name.c_str(), name.c_str(), NBIN, min, max);
	vbraggCurv.push_back(bc);

	return;

}

