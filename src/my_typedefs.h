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
	raggio_curvatura = 0, //important 0 because it will be used to access an array
	bending_angle,
	thickness,
	// A little trick: as long as the elements of the enum are numbered progressively from zero,
	// numoffields contains the number of elements preceding it.
	// The enum itself is +1 bigger than the data fields but this should not be a problem
	numoffields
};

// 4 as the number of fields in the enum above
using CrystalDataTable = std::map<std::string, std::array<Double_t, (int)FieldCrystalDataTable::numoffields> >;




// Data for crystals d
enum class FieldCrystalDataTable510 {
	raggio_curvatura5 = 0, //important 0 because it will be used to access an array
	raggio_curvatura5_err,
	raggio_curvatura10,
	raggio_curvatura10_err,
	bending_angle5,
	bending_angle5_err,
	bending_angle10,
	bending_angle10_err,
	thickness,
	dechanneling_lenght5,
	dechanneling_lenght5_err,
	dechanneling_lenght10,
	dechanneling_lenght10_err,
	// A little trick: as long as the elements of the enum are numbered progressively from zero,
	// numoffields contains the number of elements preceding it.
	// The enum itself is +1 bigger than the data fields but this should not be a problem
	numoffields
};


// Data for crystals distinguishing cuts at 5 and 10 microrads.
// NB 6 as the number of fields in the enum above
using CrystalDataTable510 = std::map<std::string, std::array<Double_t, (int)FieldCrystalDataTable510::numoffields> >;


/*
 * Unita' di misura
 */
//Fattori
constexpr Double_t CENTI_ = 1e-2;
constexpr Double_t MILLI_ = 1e-3;
constexpr Double_t MICRO_ = 1e-6;
constexpr Double_t NANO_ = 1e-9;

// Lunghezza (basata sui metri)
//constexpr m_ = 1;
//constexpr mm_ = 1e-3;
//constexpr

}

#endif /* MY_TYPEDEFS_H_ */
