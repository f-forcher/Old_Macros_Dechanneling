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
constexpr Double_t TERA_ = 1e12;
constexpr Double_t GIGA_ = 1e9;
constexpr Double_t MEGA_ = 1e6;
constexpr Double_t KILO_ = 1e3;
constexpr Double_t CENTI_ = 1e-2;
constexpr Double_t MILLI_ = 1e-3;
constexpr Double_t MICRO_ = 1e-6;
constexpr Double_t NANO_ = 1e-9;

// Conversioni
constexpr Double_t ELECTRONVOLT_ = 1.60217656535e-19; // ElectronVolt in Joule (1 eV = 1.602176565(35)*10−19 J)
constexpr Double_t ANGSTROM_ = 1e-9; // Angstrom in metri (1 Ang = 1e-9 m)

/* electronic dechanneling lenght, analytic expression
 * LDe = 256/Pi^2 * p*v/ln(2*m_e*c^2*gamma/I) * a_tf * d_p / (Z_i * r_e * m_e * c^2)
 * With
 *   p: incoming particle momentum
 *   v: speed of incoming particle
 *   m_e: electron mass
 *   c: speed of light
 *   gamma: relativistic gamma of incoming particle
 *   I: Mean Excitation Energy (like in http://pdg.lbl.gov/2016/AtomicNuclearProperties/HTML/silicon_Si.html)
 *   a_tf: Thomas-Fermi atomic radius
 *   d_p: interatomic distance
 *   @Z_i: electric charge of incoming particle
 *   r_e: classical electron radius
 *
 *   @E: incoming particle energy [eV]
 */
constexpr Double_t electronic_dechanneling(UShort_t Z, Double_t E_eV) {
	//Constants from NIST
	Double_t c = 299792458;
	Double_t m_e = 9.10938356e-31; // Electron mass [kg]
	Double_t I  = 173.0 * ELECTRONVOLT_; // I: Mean Excitation Energy [Joule]
	//Constants from D. Mirarchi, "Crystal Collimation for LHC"

	Double_t d_p = 1.92*ANGSTROM_; //d_p: interatomic distance [m]
	//Doublet_t Z

	Double_t E = E_eV * GIGA_*ELECTRONVOLT_; // Energy in [Joule]


	return 0;
}


}

#endif /* MY_TYPEDEFS_H_ */
