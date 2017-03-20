/*
 * flip_histo_file.C
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

#include "dbg_macro.h"
#include "flip_histo.h"
#include "flip_histo_vert.h"
#include "my_typedefs.h"

extern char PROJECT_DIR[];

namespace mions {
using std::endl;
using std::cout;
using std::clog;
using std::cerr;


/*
 * Function to flip a 2D histogram (either vertically or horizontally) in a file
 * @params:
 *
 * @return: 0 if success, some err code otherwise
 *
 */
int flip_histo_file(std::string name_file, std::string name_histo_toflip, HistogramFlippingDirections flipdir) {

	DBG(clog << "[LOG]: flipping histogram \"" << name_histo_toflip << "\" in file\""<< name_file << "\"" << endl;, ;)

	using EFD = HistogramFlippingDirections;
	using std::string;
	// Steps to modify an histo in a root file
	// 1) Open the file in "UPDATE"
	// 2) Check if the file has been opened.
	// 3) cd() into the file.
	// 4) Get the histo
	// 5) Modify it
	// 6) Write it
	// 7) Close the file

	// 1) Open the file in "UPDATE"
	auto file_root = new TFile(name_file.c_str(),"UPDATE");
	// 2) Check if the file has been opened.
	if (file_root->IsZombie()) {
	   std::cerr << "[ERROR]: Error opening file" << std::endl;
	   return -1;
	}
	// 3) cd() into the file.
	file_root->cd();
	// 4) Get the histo
	TH2* histo_toflip = (TH2*)file_root->Get(name_histo_toflip.c_str());
	if (histo_toflip == nullptr) {
		cerr << "[ERROR]: histogram not found in the file!";
		return -2;
	}


	// 5) Modify it

	// Change the name of the old histo and write it back
	std::string name_oldhisto_newname = name_histo_toflip + string("_old_flipped");
	histo_toflip->SetName(name_oldhisto_newname.c_str());
	histo_toflip->Write();

	//Delete with the old name and...
	std::string name_oldhisto_todelete = name_histo_toflip + string(";*");
	file_root->Delete(name_oldhisto_todelete.c_str());

	// Check in which direction(s) to flip the histograms
	TH2* new_histo;
	TH2* new_histo_horiz; //Temporary histo for the double flip
	switch (flipdir) {
		case EFD::identity:
			DBG(clog << "[LOG]: leaving histogram unchanged" << endl;, ;)
			new_histo = (TH2*)histo_toflip->Clone();// Do "nothing"

			// Name the new histo as the old one
			new_histo->SetName(name_histo_toflip.c_str());

			// 6) Write it
			new_histo->Write();

			break;
		case EFD::horizontal:
			DBG(clog << "[LOG]: flipping histogram horizontally" << endl;, ;)
			new_histo = flip_histo(histo_toflip);// Flip it horizontally

			// Name the new histo as the old one
			new_histo->SetName(name_histo_toflip.c_str());

			// 6) Write it
			new_histo->Write();

			break;
		case EFD::vertical:
			DBG(clog << "[LOG]: flipping histogram vertically" << endl;, ;)
			new_histo = flip_histo_vert(histo_toflip);// Flip it vertically

			// Name the new histo as the old one
			new_histo->SetName(name_histo_toflip.c_str());

			// 6) Write it
			new_histo->Write();

			break;
		case EFD::horizontal_vertical:
			DBG(clog << "[LOG]: flipping histogram both horizontally and vertically" << endl;, ;)
			new_histo_horiz = flip_histo(histo_toflip); // First flip it horizontally...
			new_histo = flip_histo_vert(new_histo_horiz);    // ...then vertically
			// Name the new histo as the old one
			new_histo->SetName(name_histo_toflip.c_str());

			// 6) Write it
			new_histo->Write();

			break;
		default:
			std::cerr << "[ERROR]: Error in the HistogramFlippingDirections enum??" << std::endl;
			file_root->Close();
			return -1;
			break;
	}

	// 7) Close the file
	file_root->Close();
	return 0;
}

}
