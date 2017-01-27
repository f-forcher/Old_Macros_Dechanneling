/*
 * analisi_VRtoAM.h
 *
 *  Created on: Nov 24, 2016
 *      Author: fforcher
 */

#ifndef ANALISI_VRTOAM_H_
#define ANALISI_VRTOAM_H_

#ifndef __CLING__

namespace mions {
/*
 * Function analysing the slices in the VR to AM transition.
 */
void analisi_VRtoAM(std::string nome_cristallo, std::string exp_or_sim, int start_analysis, int analysis_width);

}

#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "analisi_VRtoAM.C"

#endif


#endif /* ANALISI_VRTOAM_H_ */
