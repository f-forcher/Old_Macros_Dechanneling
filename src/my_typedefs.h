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
#include <cmath>

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
	slopeDc5,
	slopeDc5_err,
	slopeDc10,
	slopeDc10_err,
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
constexpr Double_t PI_ = 3.141592653589793238463;

// Conversioni
constexpr Double_t ELECTRONVOLT_ = 1.60217656535e-19; // ElectronVolt in Joule (1 eV = 1.602176565(35)*10−19 J)
constexpr Double_t ANGSTROM_ = 1e-10; // Angstrom in metri (1 Ang = 1e-9 m)

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

constexpr Double_t electronic_dechanneling(UShort_t Z, Double_t E_GeV) {
	using std::pow;
	using std::log;
	using std::clog;
	using std::endl;
	//Constants from NIST
	Double_t c = 299792458; //Speed of light [m/s]
	Double_t r_e = 2.8179403227e-15; //Classical electron radius [m]
	// Double_t c_p = 1.60217733e-19; // Carica protone [Coul]
	Double_t m_e = 9.10938356e-31; // Electron mass [kg]
	// Double_t m_e_ev = m_e / (1 / (c * c) * ELECTRONVOLT_); //Electron mass [eV] ~ 511000 ev
	Double_t m_p = 1.672621898e-27; //Proton mass [Kg]
	// Double_t m_p_ev = m_p / (1 / (c * c) * ELECTRONVOLT_); //Proton mass [eV] ~ 938272 ev
	//DBG( std::clog << "m_e: " << m_e << std::endl; , ; );
	Double_t I = 173.0 * ELECTRONVOLT_; // I: Mean Excitation Energy [Joule]

	//Constants from D. Mirarchi, "Crystal Collimation for LHC", and R. Rossi Master Thesis
	Double_t d_p = 1.92 * ANGSTROM_; //d_p: interatomic distance [m]
	Double_t Z_Si = 14; // Integer Electric charge of the incoming particle
	Double_t E = E_GeV * GIGA_ * ELECTRONVOLT_; // Energy in [Joule]
	Double_t a_tf = 0.8853 * (0.529 * ANGSTROM_) * pow( Z + Z_Si, -1.0 / 3.0 ); // Thomas-Fermi Radius [m?]: (0.529*ANGSTROM_) bohr radius

	//Double_t pv = 400*GIGA_*ELECTRONVOLT_; //Energy in GeV
	Double_t gamma = E / (m_p * c * c);
	//clog << gamma << endl;
	Double_t LDe = 256.0 / (9 * PI_ * PI_) * E / (log( 2 * m_e * c * c * gamma * pow( I, -1.0 ) ) - 1) * (a_tf * d_p)
			/ (Z * r_e * m_e * c * c);

	return LDe;
}

} // mions

#endif /* MY_TYPEDEFS_H_ */
