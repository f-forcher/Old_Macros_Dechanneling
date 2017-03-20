/*
 * electronic_dechanneling.C
 *
 *  Created on: Mar 20, 2017
 *      Author: fforcher
 */

#include <iostream>
#include "my_typedefs.h"

namespace mions {

double electronic_dechanneling(unsigned short Z, double E_GeV) {
	using std::pow;
	using std::log;
	using std::clog;
	using std::endl;
	//Constants from NIST
	double c = 299792458; //Speed of light [m/s]
	double r_e = 2.8179403227e-15; //Classical electron radius [m]
	// double c_p = 1.60217733e-19; // Carica protone [Coul]
	double m_e = 9.10938356e-31; // Electron mass [kg]
	// double m_e_ev = m_e / (1 / (c * c) * ELECTRONVOLT_); //Electron mass [eV] ~ 511000 ev
	double m_p = 1.672621898e-27; //Proton mass [Kg]
	// double m_p_ev = m_p / (1 / (c * c) * ELECTRONVOLT_); //Proton mass [eV] ~ 938272 ev
	//DBG( std::clog << "m_e: " << m_e << std::endl; , ; );
	double I = 173.0 * ELECTRONVOLT_; // I: Mean Excitation Energy [Joule]

	//Constants from D. Mirarchi, "Crystal Collimation for LHC", and R. Rossi Master Thesis
	double d_p = 1.92 * ANGSTROM_; //d_p: interatomic distance [m]
	double Z_Si = 14; // Integer Electric charge of the incoming particle
	double E = E_GeV * GIGA_ * ELECTRONVOLT_; // Energy in [Joule]
	double a_tf = 0.8853 * (0.529 * ANGSTROM_) * pow( Z + Z_Si, -1.0 / 3.0 ); // Thomas-Fermi Radius [m?]: (0.529*ANGSTROM_) bohr radius

	//double pv = 400*GIGA_*ELECTRONVOLT_; //Energy in GeV
	double gamma = E / (m_p * c * c);
	//clog << gamma << endl;
	double LDe = 256.0 / (9 * PI_ * PI_) * E / (log( 2 * m_e * c * c * gamma * pow( I, -1.0 ) ) - 1) * (a_tf * d_p)
			/ (Z * r_e * m_e * c * c);

	return LDe;
}


}
