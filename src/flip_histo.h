/*
 * flip_histo.h
 *
 *  Created on: Mar 15, 2017
 *      Author: fforcher
 */

#ifndef FLIP_HISTO_H_
#define FLIP_HISTO_H_

#include <TH2.h>

// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__
// Se non siamo nell'interprete, ma stiamo compilando, metti la forward declaration perche' verra' linkata quando compili eclipse
namespace mions {


// Macro to flip an histogram around his Y axis, in the X direction.
TH2* flip_histo(TH2*& histogram);


#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "slices_horiz.C"

#endif


#endif /* FLIP_HISTO_H_ */
