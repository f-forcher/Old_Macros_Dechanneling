/*
 * AnalisiVRAM_param.h
 *
 *  Created on: Jan 26, 2017
 *      Author: fforcher
 */

#ifndef ANALISIVRAM_PARAM_H_
#define ANALISIVRAM_PARAM_H_

namespace mions {

#include <vector>
#include <string>

/*
 * Struct containing the parameters for transition region identification,
 */
struct AnalisiVRAM_param {

	int start_analysis;
	int analysis_width;
	int deltaslice;

	std::map<std::string, double> other_params;
};


}



#endif /* ANALISIVRAM_PARAM_H_ */
