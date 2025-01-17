/*
 * slices_horiz.h
 *
 *  Created on: Feb 20, 2017
 *      Author: fforcher
 */

#ifndef SLICES_HORIZ_H_
#define SLICES_HORIZ_H_

#include <string>
#include <map>

// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__
// Se non siamo nell'interprete, ma stiamo compilando, metti la forward declaration perche' verra' linkata quando compili eclipse
namespace mions {


//void slices(string nome_cristallo, string pathfiledati_root, Double_t cut1, Double_t cut2, TH1D*& hist, bool makepng = false);
void slices_horiz(std::string nome_cristallo, std::string exp_or_sim,
		Double_t cut1, Double_t cut2,
		TH1D*& hist, std::map<std::string, double> otherparams = {}, bool makepng = false);

}
#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "slices_horiz.C"

#endif


#endif /* SLICES_HORIZ_H_ */
