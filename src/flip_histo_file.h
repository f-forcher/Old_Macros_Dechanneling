/*
 * flip_histo_file.h
 *
 *  Created on: Mar 15, 2017
 *      Author: fforcher
 */

#ifndef FLIP_HISTO_FILE_H_
#define FLIP_HISTO_FILE_H_

#include <TH2.h>
#include "my_typedefs.h"

// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__
// Se non siamo nell'interprete, ma stiamo compilando, metti la forward declaration perche' verra' linkata quando compili eclipse
namespace mions {


/*
 * Function to flip a 2D histogram (either vertically or horizontally) in a file
 * It will replace the old histogram with the new one, saving a copy of the old one under the name of name_histo_toflip + "before_flipping"
 * @params:
 * 			name_file: name of the file to turn around
 *			name_histo_toflip: name of the histogram to flip
 *			flipdir: a membeof the HistogramFlippingDirections::identity/horizontal/vertical/horizontal_vertical enum class, indicating
 *					 in which direction(s) to flip the file
 * @return: 0 if success, some err code otherwise
 *
 */
int flip_histo_file(std::string name_file, std::string name_histo_toflip, HistogramFlippingDirections flipdir);

}
#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "flip_histo_file.C"

#endif


#endif /* FLIP_HISTO_H_ */
