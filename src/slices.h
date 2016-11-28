/*
 * slices.h
 *
 *  Created on: Nov 23, 2016
 *      Author: fforcher
 */

#ifndef SLICES_H_
#define SLICES_H_

// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__
// Se non siamo nell'interprete, ma stiamo compilando, metti la forward declaration perche' verra' linkata quando compili eclipse
namespace mions {


void slices(Double_t cut1, Double_t cut2, TH1D*& hist, bool makepng = false);

}
#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "slices.C"

#endif


#endif /* SLICES_H_ */
