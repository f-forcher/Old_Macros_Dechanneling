import matplotlib.pyplot as plt
plt.ion()
import matplotlib.mlab
from matplotlib.markers import MarkerStyle
import numpy as np
from pylab import *


# "Main"
crystal_name = sys.argv[1]
from_slice = int(sys.argv[2])
to_slice = int(sys.argv[3])

crystal_params_fields = ("Crystal,raggio_curvatura5[m],raggio_curvatura5_err[m],"
"raggio_curvatura10[m],raggio_curvatura10_err[m],bending_angle5[microrad],bending_angle5_err[microrad],"
"bending_angle10[microrad],bending_angle10_err[microrad],thickness[mm],slopeDc5[1/microrad],"
"slopeDc5_err[1/microrad],slopeDc10[1/microrad],slopeDc10_err[1/microrad],dechanneling_lenght5[m],"
"dechanneling_lenght5_err[m],dechanneling_lenght10[m],dechanneling_lenght10_err[m],particle_type,particle_energy[GeV]").split(',')
crystal_params_raw = np.genfromtxt("crystal_calc_table.txt",
dtype="a5,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8,f8",
names=crystal_params_fields
)

# Double dict comphrehension!!
# Make a double entry table out of the crystal_params_raw numpy array, i.e.
# crystal_params['STF45']['raggio_curvatura5[m]'] = 13.8248
crystal_params = {row[0].decode("utf-8"): {crystal_params_fields[i]: row[i] for i in range(len(row)) } for row in crystal_params_raw}



particle_energy = crystal_params[crystal_name]['particle_energy[GeV]'] * 1e9 # [eV] TODO generalize to pions!
critical_radius = 1 #[m] TODO at 400 GeV
pot_well = 21.34 #[eV] Potential well between crystal planes
theta_bending = crystal_params[crystal_name]['bending_angle10[microrad]']
crystal_curvature_radius = crystal_params[crystal_name]['raggio_curvatura10[m]']
theta_c = math.sqrt(2*pot_well/particle_energy) * (1 - critical_radius/crystal_curvature_radius)*1e6 # [murad]
c1_thetavr, c2_thetavr = (-1.5, 1.66666)
theta_vr =  c1_thetavr * theta_c * (1 - c2_thetavr*critical_radius/crystal_curvature_radius) # [murad]












plt.figure()



plt.axvline(x=theta_bending, linestyle="dashed", color='Crimson', label=r"$\theta_{b}$")
plt.axvline(x=theta_bending + theta_c, linestyle="dashed", label=r"$\theta_{b}  + \theta_c$")
plt.axvline(x=theta_bending + 2*theta_c, linestyle="dashed", label=r"$\theta_{b}  + 2 \theta_c$")

plt.plot([from_slice,theta_bending], [1,1], linestyle="solid", label="", color='Navy')
plt.plot([theta_bending,theta_bending + 2*theta_c], [1,0], linestyle="solid", label="Simulation VR weights (without noise)", color='Navy')
plt.plot([theta_bending + 2*theta_c,to_slice], [0,0], linestyle="solid", label="", color='Navy')

plt.plot([from_slice ,theta_bending], [0,0], linestyle="solid", label="", color='DarkOrange')
plt.plot([theta_bending,theta_bending + 2*theta_c], [0,1], linestyle="solid", label="Simulation AM weights (without noise)", color='DarkOrange')
plt.plot([theta_bending + 2*theta_c,to_slice], [1,1], linestyle="solid", label="", color='DarkOrange')


plt.title("Theoretical model of the transition (STF45)")
plt.xlabel(r'$\theta_{x}\ [\mu rad]$')
plt.ylabel('Probability')
plt.legend()
plt.show()
