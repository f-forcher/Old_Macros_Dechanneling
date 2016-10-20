/*
 * dech.h
 *
 *  Created on: Sep 27, 2016
 *      Author: fforcher
 */

#ifndef DECH_H_
#define DECH_H_

// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__
// Se non siamo nell'interprete, ma stiamo compilando, metti la forward declaration perche' verra' linkata quando compili eclipse
void dech(std::string nome_cristallo, std::shared_ptr<std::ofstream> output_dech);

#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "dech.C"

#endif

#endif /* DECH_H_ */
