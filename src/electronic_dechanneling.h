/*
 * mia_dech.h
 *
 *  Created on: Sep 27, 2016
 *      Author: fforcher
 */

#ifndef ELECTRONIC_DECHANNELING_H_
#define ELECTRONIC_DECHANNELING_H_

// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__

#include "my_typedefs.h"

namespace mions {

//Function that calculate lectronic dechanneling lenght (in meters)
double electronic_dechanneling(unsigned short Z, double E_GeV);

}

#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "electronic_dechanneling.C"

#endif

#endif /* ELECTRONIC_DECHANNELING_H_ */
