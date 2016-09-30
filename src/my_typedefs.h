/*
 * my_typedefs.h
 *
 *  Created on: Sep 30, 2016
 *      Author: fforcher
 */

#ifndef MY_TYPEDEFS_H_
#define MY_TYPEDEFS_H_

#include <Rtypes.h>
#include <map>
#include <array>
#include <string>

namespace mions {

enum class FieldCrystalDataTable {
	raggio_curvatura = 0, bending_angle = 1
};

// 2 as the number of fields in the enum above
using CrystalDataTable = std::map<std::string, std::array<Double_t, 2> >;

}

#endif /* MY_TYPEDEFS_H_ */
