/*
 * flip_histo_vert.C
 *
 *  Created on: Mar 15, 2017
 *      Author: fforcher
 */

#include <cmath>
#include <iostream>


#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <TCanvas.h>

#include "./dbg_macro.h"
//#include "./my_typedefs.h"

extern char PROJECT_DIR[];

namespace mions {
using std::endl;
using std::cout;
using std::clog;
using std::cerr;


/*
 * Code to flip a 2D histogram vertically (around the X axis):
 * @params:
 * 			histogram: the histogram to flip
 * @return: the horizontally flipped histogram
 *
 */
TH2* flip_histo_vert(TH2*& histogram) {
	auto name_orig = histogram->GetName();
	auto title_orig = histogram->GetTitle();
	TAxis* xAxis_orig = histogram->GetXaxis();
	TAxis* yAxis_orig = histogram->GetYaxis();
	auto numXbin = histogram->GetNbinsX();
	auto numYbin = histogram->GetNbinsY();
	auto xMin_orig = xAxis_orig->GetXmin();
	auto xMax_orig = xAxis_orig->GetXmax();
	auto yMin_orig = yAxis_orig->GetXmin(); // Yeah it's called GetXmin() even for a generic TAxis...
	auto yMax_orig = yAxis_orig->GetXmax(); // Just why, ROOT, why...

	TH2* flipped_histo = new TH2D(
	/* name */ name_orig,
	/* title */ title_orig,
	/* X-dimension */ numXbin, xMin_orig, xMax_orig, //Flipped x-axis
	/* Y-dimension */ numYbin, -yMax_orig, -yMin_orig );

	//In root, bin=0 is the underflow, and then bin = nBins + 1 is the overflow
	for (auto i = 0; i <= numXbin + 1; i++) {
		for (auto j = 0; j <= numYbin + 1; j++) {
			auto old_val_ij = histogram->GetBinContent( i, j );
			//swap
			flipped_histo->SetBinContent(i, numYbin + 1 - j, old_val_ij);
		}
	}


	return flipped_histo;
}

}
