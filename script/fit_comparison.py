#!/bin/python3
from sklearn import mixture
import sys
import matplotlib.pyplot as plt
plt.ion()
import matplotlib.mlab
from matplotlib.markers import MarkerStyle
import numpy as np
from pylab import *
import random
import math  #erf
import collections

import pprint
pp = pprint.PrettyPrinter(indent=4)

# "Main"
crystal_name = sys.argv[1]
quale_sim = sys.argv[2]
from_slice = int(sys.argv[3])  # [murad] Has to be an existing slice, for now
to_slice = int(sys.argv[4])
#dat_or_root = sys.argv[5]
# Crystal orientation:
# Triangle pointing to the right ="R"
#  **
#   | \
# --___--
# Triangle pointing to the left ="L"
#      **
#    / |
# --___--
crystal_orientation = "R"

deltaslice = 1

# Calculate the various thetas for the crystal

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

data_folder_exp = "ForFrancesco/" + crystal_name + "_" + "exp" + "/txt_data/"
data_folder_sim = "ForFrancesco/" + crystal_name + "_" + quale_sim + "/txt_data/"

cur_slice = from_slice

# Maps/dicts containing the relative weights of each mixture with cur_slice as key
# (real) example:
#        weights_AM = {-190: 0.73387685, -188: 0.681326581759, ...}
#        weights_VR = {-190: 0.26612315, -188: 0.318673418241, ...}
weights_AM_exp = collections.OrderedDict()
weights_VR_exp = collections.OrderedDict()
means_AM_exp   = collections.OrderedDict()
means_VR_exp   = collections.OrderedDict()
sigma2s_AM_exp = collections.OrderedDict()
sigma2s_VR_exp = collections.OrderedDict() #sigma2s_AM and sigma2s_VR should be equal if we fit with tied covariance (s1=s2)

weights_AM_sim = collections.OrderedDict()
weights_VR_sim = collections.OrderedDict()
means_AM_sim   = collections.OrderedDict()
means_VR_sim   = collections.OrderedDict()
sigma2s_AM_sim = collections.OrderedDict()
sigma2s_VR_sim = collections.OrderedDict() #sigma2s_AM and sigma2s_VR should be equal if we fit with tied covariance (s1=s2)



# FIT EXP
clf = mixture.GaussianMixture(
    n_components=2,
    covariance_type='tied',
    verbose=2,
    verbose_interval=10,
    random_state=random.SystemRandom().randrange(0, 4095),
    means_init=[[-17], [0]],
    #                              weights_init=[1 / 2, 1 / 2],
    init_params="kmeans",
    tol=1e-5,
    #warm_start=True,
    max_iter=1000)
data_folder = data_folder_exp
weights_AM = weights_AM_exp
weights_VR = weights_VR_exp
means_AM   = means_AM_exp
means_VR   = means_VR_exp
sigma2s_AM = sigma2s_AM_exp
sigma2s_VR = sigma2s_VR_exp
while (cur_slice < to_slice):

    slice_name = "Slices_" + str(cur_slice) + "_" + str(
        cur_slice + deltaslice) + "_" + crystal_name
    data = np.loadtxt(data_folder_exp + slice_name + ".txt")
    #data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-186_-184_ST101.txt')
    #data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-142_-140_ST101.txt')

    x = data[:, 0]
    y = data[:, 1]

    # We have the data histogram, a 2D array with the sample density for every
    # x-centered bin, like
    # [
    #    [-38, 2],
    #    [-36, 1],
    #    [-34, 3],
    #    ...
    # ]
    # But scikit want samples, something like
    # [
    #    [-38],
    #    [-38],
    #    [-36],
    #    [-34],
    #    [-34],
    #    [-34],
    #    ...
    # ]
    # So we create (deterministic) samples from the histogram
    distribution = []
    for i in range(len(y)):
        posizione_col = x[i]
        altezza_col = y[i]
        for j in range(int(np.round(altezza_col))):
            distribution.append(x[i])

    #pp.pprint(distribution)
    # Make the numpy array
    nd_distribution = np.array(distribution)

    # Create normalized histogram
    yn = y / np.sum(y)

    # Fit the two peaks
    clf.fit(nd_distribution.reshape(-1, 1))

    # "Unflattened" variables
    r_m1, r_m2 = clf.means_
    w1, w2 = clf.weights_
    #    r_c1, r_c2 = clf.covariances_
    r_c1 = clf.covariances_
    r_c2 = clf.covariances_
    print("means_: ", clf.means_)
    print("weights_: ", clf.weights_)
    print("covariances_: ", clf.covariances_)

    print("First peak: ", r_m1[0], "Second peak:", r_m2[0])
    print("w1: ", w1, "w2:", w2)
    print("c1: ", r_c1[0][0], "c2:", r_c2[0][0])

    # Flatten variables
    m1, m2 = r_m1[0], r_m2[0]
    c1, c2 = r_c1[0][0], r_c2[0][0]

    # m1, m2 = r_m1, r_m2
    # c1, c2 = r_c1, r_c2

    # Save the weights in the right array
    # Lower Y is the VR peak
    if (m1 < m2):
        weights_VR[cur_slice] = w1
        weights_AM[cur_slice] = w2
        means_VR[cur_slice] = m1
        means_AM[cur_slice] = m2
        sigma2s_VR[cur_slice] = c1
        sigma2s_AM[cur_slice] = c2
    else:
        weights_VR[cur_slice] = w2
        weights_AM[cur_slice] = w1
        means_VR[cur_slice] = m2
        means_AM[cur_slice] = m1
        sigma2s_VR[cur_slice] = c2
        sigma2s_AM[cur_slice] = c1

    #fig = plt.figure(figsize = (5, 5))
    #plt.subplot(111)
    # gauss_test = 0.5*matplotlib.mlab.normpdf(x, -20, 8)
    gauss1 = w1 * matplotlib.mlab.normpdf(x, m1, np.sqrt(c1))
    gauss2 = w2 * matplotlib.mlab.normpdf(x, m2, np.sqrt(c2))
    gauss_tot = gauss1 + gauss2

    plt.plot(
        x,
        yn,
        linestyle="dashed",
        drawstyle="steps-mid",
        label="Data",
        color='b')
    plt.plot(x, gauss1, label="Gauss1", color='g')
    plt.plot(x, gauss2, label="Gauss2", color='g')
    plt.plot(x, gauss_tot, label="Gauss_tot", color='r')

    plt.legend()
    #plt.show()

    # plt.figure()  # New window, if needed.  No need to save it, as pyplot uses the concept of current figure
    # plt.plot(np.random.rand(10))
    # plt.show()

    figure_folder = "script/python_video/" + crystal_name + "_exp" + "/"
    #plt.savefig(figure_folder + "gaussian_fit.png", dpi=300)

    plt.savefig(figure_folder + slice_name + ".png", dpi=300)
    plt.clf()

    # Update cur_slice counter. Yeah maybe there's a more snakey way, don't care for now
    cur_slice = cur_slice + deltaslice
# END FIT EXP

# FIT SIM
cur_slice = from_slice
clf = mixture.GaussianMixture(
    n_components=2,
    covariance_type='tied',
    verbose=2,
    verbose_interval=10,
    random_state=random.SystemRandom().randrange(0, 4095),
    means_init=[[-17], [0]],
    #                              weights_init=[1 / 2, 1 / 2],
    init_params="kmeans",
    tol=1e-5,
    max_iter=1000)
data_folder = data_folder_sim
weights_AM = weights_AM_sim
weights_VR = weights_VR_sim
means_AM   = means_AM_sim
means_VR   = means_VR_sim
sigma2s_AM = sigma2s_AM_sim
sigma2s_VR = sigma2s_VR_sim
while (cur_slice < to_slice):

    slice_name = "Slices_" + str(cur_slice) + "_" + str(
        cur_slice + deltaslice) + "_" + crystal_name
    data = np.loadtxt(data_folder + slice_name + ".txt")
    #data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-186_-184_ST101.txt')
    #data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-142_-140_ST101.txt')

    x = data[:, 0]
    y = data[:, 1]

    # We have the data histogram, a 2D array with the sample density for every
    # x-centered bin, like
    # [
    #    [-38, 2],
    #    [-36, 1],
    #    [-34, 3],
    #    ...
    # ]
    # But scikit want samples, something like
    # [
    #    [-38],
    #    [-38],
    #    [-36],
    #    [-34],
    #    [-34],
    #    [-34],
    #    ...
    # ]
    # So we create (deterministic) samples from the histogram
    distribution = []
    for i in range(len(y)):
        posizione_col = x[i]
        altezza_col = y[i]
        for j in range(int(np.round(altezza_col))):
            distribution.append(x[i])

    #pp.pprint(distribution)
    # Make the numpy array
    nd_distribution = np.array(distribution)

    # Create normalized histogram
    yn = y / np.sum(y)

    # Fit the two peaks
    clf.fit(nd_distribution.reshape(-1, 1))

    # "Unflattened" variables
    r_m1, r_m2 = clf.means_
    w1, w2 = clf.weights_
    #    r_c1, r_c2 = clf.covariances_
    r_c1 = clf.covariances_
    r_c2 = clf.covariances_
    print("means_: ", clf.means_)
    print("weights_: ", clf.weights_)
    print("covariances_: ", clf.covariances_)

    print("First peak: ", r_m1[0], "Second peak:", r_m2[0])
    print("w1: ", w1, "w2:", w2)
    print("c1: ", r_c1[0][0], "c2:", r_c2[0][0])

    # Flatten variables
    m1, m2 = r_m1[0], r_m2[0]
    c1, c2 = r_c1[0][0], r_c2[0][0]

    # m1, m2 = r_m1, r_m2
    # c1, c2 = r_c1, r_c2

    # Save the weights in the right array
    # Lower Y is the VR peak
    if (m1 < m2):
        weights_VR[cur_slice] = w1
        weights_AM[cur_slice] = w2
        means_VR[cur_slice] = m1
        means_AM[cur_slice] = m2
        sigma2s_VR[cur_slice] = c1
        sigma2s_AM[cur_slice] = c2
    else:
        weights_VR[cur_slice] = w2
        weights_AM[cur_slice] = w1
        means_VR[cur_slice] = m2
        means_AM[cur_slice] = m1
        sigma2s_VR[cur_slice] = c2
        sigma2s_AM[cur_slice] = c1

    #fig = plt.figure(figsize = (5, 5))
    #plt.subplot(111)
    # gauss_test = 0.5*matplotlib.mlab.normpdf(x, -20, 8)
    gauss1 = w1 * matplotlib.mlab.normpdf(x, m1, np.sqrt(c1))
    gauss2 = w2 * matplotlib.mlab.normpdf(x, m2, np.sqrt(c2))
    gauss_tot = gauss1 + gauss2

    plt.plot(
        x,
        yn,
        linestyle="dashed",
        drawstyle="steps-mid",
        label="Data",
        color='b')
    plt.plot(x, gauss1, label="Gauss1", color='g')
    plt.plot(x, gauss2, label="Gauss2", color='g')
    plt.plot(x, gauss_tot, label="Gauss_tot", color='r')

    plt.legend()
    #plt.show()

    # plt.figure()  # New window, if needed.  No need to save it, as pyplot uses the concept of current figure
    # plt.plot(np.random.rand(10))
    # plt.show()

    figure_folder = "script/python_video/" + crystal_name + "_" + quale_sim + "/"
    #plt.savefig(figure_folder + "gaussian_fit.png", dpi=300)

    plt.savefig(figure_folder + slice_name + ".png", dpi=300)
    plt.clf()

    # Update cur_slice counter. Yeah maybe there's a more snakey way, don't care for now
    cur_slice = cur_slice + deltaslice
# END FIT SIM


print("weights_AM:")
pp.pprint(weights_AM)
print("weights_VR:")
pp.pprint(weights_VR)

# Fit weights/proportions
from scipy.optimize import curve_fit
from scipy.special import erf


def erf_to_fit(xx, m, x0):
    return 0.5 * erf(m * (xx - x0)) + 0.5

# Only exp are a parabola,
def parabola_to_fit(xx, a, b, c):
    return a*xx*xx + b*xx + c

x_AM_exp = list(weights_AM_exp.keys())
y_AM_exp = list(weights_AM_exp.values())
x_VR_exp = list(weights_VR_exp.keys())
y_VR_exp = list(weights_VR_exp.values())

y_meansAM_exp = list(means_AM_exp.values())
y_meansVR_exp = list(means_VR_exp.values())

y_sigmas_exp = [np.sqrt(xx) for xx in sigma2s_VR_exp.values()]

x_AM_sim = list(weights_AM_sim.keys())
y_AM_sim = list(weights_AM_sim.values())
x_VR_sim = list(weights_VR_sim.keys())
y_VR_sim = list(weights_VR_sim.values())

y_meansAM_sim = list(means_AM_sim.values())
y_meansVR_sim = list(means_VR_sim.values())

y_sigmas_sim = [np.sqrt(xx) for xx in sigma2s_VR_sim.values()]


AM_parameters_exp, AM_par_covars_exp = curve_fit(
    erf_to_fit, x_AM_exp, y_AM_exp, p0=[1, (from_slice + to_slice) / 2])
AM_parameters_sim, AM_par_covars_sim = curve_fit(
    erf_to_fit, x_AM_sim, y_AM_sim, p0=[1, (from_slice + to_slice) / 2])

# Only exp data
Means_parameters, Means_params_covars = curve_fit(
    parabola_to_fit, x_AM_exp, y_meansAM_exp, p0=None)



#x_fitAM = np.linspace(-(interceptAM - 1) / slopeAM, -interceptAM / slopeAM, 100)
y_fitAM_exp = [erf_to_fit(xx, AM_parameters_exp[0], AM_parameters_exp[1]) for xx in x_AM_exp]
y_fitVR_exp = [1 - yy for yy in y_fitAM_exp]

y_fitAM_sim = [erf_to_fit(xx, AM_parameters_sim[0], AM_parameters_sim[1]) for xx in x_AM_sim]
y_fitVR_sim = [1 - yy for yy in y_fitAM_sim]

y_simgen = [erf_to_fit(xx, 1/(1.41421356*2*theta_c), theta_bending + theta_c) for xx in x_AM_sim]

y_fitMeansAM = [parabola_to_fit(xx, Means_parameters[0], Means_parameters[1], Means_parameters[2]) for xx in x_AM_exp]

# http://matplotlib.org/api/markers_api.html marker numbers
if crystal_orientation == "R":
    marker_AM = 6
    marker_VR = 11
    or_sign = 1 # Orientation sign, to plot correctly the theta vertical bars
elif crystal_orientation == "L":
    marker_AM = 11
    marker_VR = 6
    or_sign = -1

# Plot results
plt.clf()
plt.plot(
    x_AM_exp,
    y_AM_exp,
    linestyle="dotted",
    marker=marker_AM,
    label="AM Data exp",
    color='DarkGreen')
plt.plot(
    x_VR_exp,
    y_VR_exp,
    linestyle="dotted",
    marker=marker_VR,
    label="VR data exp",
    color='DarkRed')
plt.plot(
    x_AM_sim,
    y_AM_sim,
    linestyle="dashdot",
    marker=marker_AM,
    label="AM Data sim",
    color='g')
plt.plot(
    x_VR_sim,
    y_VR_sim,
    linestyle="dashdot",
    marker=marker_VR,
    label="VR data sim",
    color='r')


plt.plot(x_AM_exp, y_fitAM_exp, linestyle="solid", label="exp AM fit", color='Navy')
plt.plot(x_VR_exp, y_fitVR_exp, linestyle="solid", label="exp VR fit", color='BlueViolet')
plt.plot(x_AM_sim, y_fitAM_sim, linestyle="dashed", label="sim AM fit", color='Navy')
plt.plot(x_VR_sim, y_fitVR_sim, linestyle="dashed", label="sim VR fit", color='BlueViolet')

plt.plot(x_VR_sim, y_simgen, linestyle="dashed", label="Simulation Generatrix", color='DarkOrange')


plt.axvline(x=or_sign*(theta_bending + theta_c*0), linestyle="dashed", color='Chartreuse')  # TODO
plt.axvline(x=or_sign*(theta_bending + theta_c*0 + theta_c), linestyle="dashed")  #TODO
plt.axvline(x=or_sign*(theta_bending + theta_c*0 + 2*theta_c), linestyle="dashed")  #TODO

plt.title(crystal_name + "_" + quale_sim + ": weights")
plt.legend()
plt.show()

# Plot (exp) means
plt.figure()
plt.axhline(y=0, linestyle="dashed")
plt.axhline(y=theta_vr, linestyle="dashed")  #TODO
plt.plot(
    x_AM_exp,
    y_meansAM_exp,
    linestyle="dotted",
    marker="+",
    label="Exp AM means",
    color='Crimson')
plt.plot(
    x_AM_exp,
    y_meansVR_exp,
    linestyle="dotted",
    marker="+",
    label="Exp VR means",
    color='RoyalBlue')
plt.plot(
    x_AM_sim,
    y_meansAM_sim,
    linestyle="dotted",
    marker="x",
    label="Sim AM means",
    color='Red')
plt.plot(
    x_AM_sim,
    y_meansVR_sim,
    linestyle="dotted",
    marker="x",
    label="Sim VR means",
    color='Blue')

plt.plot(
    x_AM_exp,
    y_fitMeansAM,
    linestyle="solid",
    label="VR means fit",
    color='Aqua')
plt.title(crystal_name + "_" + quale_sim + ": means")
plt.legend()
plt.show()

# Plot sigmas
plt.figure()
plt.plot(
    x_AM_exp,
    y_sigmas_exp,
    linestyle="solid",
    label="Experimental Sigmas",
    color='Blue')
plt.plot(
    x_AM_sim,
    y_sigmas_sim,
    linestyle="dashed",
    label="Simulation Sigmas",
    color='BlueViolet')
plt.title(crystal_name + "_" + quale_sim + ": sigmas")
plt.legend()
plt.show()

print("Exp weights erf fit parameters: ")
print("m: ", AM_parameters_exp[0], "-> c=1/m: ", 1 / AM_parameters_exp[0])
print("x0: ", AM_parameters_exp[1])
print("Exp covariance matrix: ")
pp.pprint(AM_par_covars_exp)
print()

print("Sim weights erf fit parameters: ")
print("m: ", AM_parameters_sim[0], "-> c=1/m: ", 1 / AM_parameters_sim[0])
print("x0: ", AM_parameters_sim[1])
print("Sim covariance matrix: ")
pp.pprint(AM_par_covars_sim)
print()


print("(Exp) Means parabola fit parameters: ")
print("a: ", Means_parameters[0])
print("b: ", Means_parameters[1])
print("c: ", Means_parameters[2])

s = input('--> ')

#fig.savefig('gaussian_fit.pdf')
