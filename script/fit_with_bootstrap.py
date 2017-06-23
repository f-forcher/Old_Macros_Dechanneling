#!/bin/python3
# equivalent to test_fit_1or2.py but with errors
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
import bisect, itertools # For the weighted sampling, from https://docs.python.org/3.5/library/random.html#examples-and-recipes

import pprint
pp = pprint.PrettyPrinter(indent=4)

def create_synth_dataset_from_histo(xhisto, yhisto, randseed=None):
    '''
    Create a synthetic dataset using drawing with replacement from the data histogram.
    x: Histogram x values (bin centers)
    y: histogram y values (bin content). Must be nonnegative integers and len(y) == len(x)
    return: a list of N data ("raw" datapoints, NOT an histogram!) values
            extracted from the real data with replacement.
    '''
    synth_dataset = []
    data_cdf = list(itertools.accumulate(yhisto)) # data_cdf[i] = sum(y[i], 0, i) https://docs.python.org/3.5/library/random.html#examples-and-recipes
    # randseed = random.SystemRandom().randrange(0, 9223372036854775807) # sys.maxsize = 2^63-1
    random.seed(randseed)
    extracted_num = random.random() * data_cdf[-1] # data_cdf[-1] = total number of events inside the histogram
    for j in range(int(np.round(data_cdf[-1]))): # data_cdf[-1] = see above comment
        extracted_num = random.random() * data_cdf[-1]
        new_extracted_elem = xhisto[bisect.bisect(data_cdf,extracted_num)]
        synth_dataset.append(new_extracted_elem)
    return synth_dataset

def transform_histo_to_dataset(xhisto,yhisto):
    '''
    Transforms (deterministically) an histogram into datapoints, because e.g.
    scikit fits datapoints and not histograms.
    x: Histogram x values (bin centers)
    y: histogram y values (bin content). Must be nonnegative integers and len(y) == len(x)
    return: a set of datapoints created as explained below

    We have the data histogram, a 2D array with the sample density for every
    x-centered bin, like
    [
       [-38, 2],
       [-36, 1],
       [-34, 3],
       ...
    ]
    But scikit want samples, something like
    [
       [-38],
       [-38],
       [-36],
       [-34],
       [-34],
       [-34],
       ...
    ]
    So we create (deterministic) samples from the histogram,
    simply by accumulating y (bin content) datapoints at each bin center x
    '''
    distribution = []
    for i in range(len(y)):
        posizione_col = x[i]
        altezza_col = y[i]
        for j in range(int(np.round(altezza_col))):
            distribution.append(posizione_col)
    return distribution


# "Main"
crystal_name = sys.argv[1]
exp_or_sim = sys.argv[2]
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
if sys.argv[5] == "R":
    crystal_orientation = "R"
elif sys.argv[5] == "L":
    crystal_orientation = "L"
else:
    raise ValueError(
        "[ERROR]: crystal_orientation (fourth CLI argument) should be either R or L!"
    )

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

data_folder = "ForFrancesco/" + crystal_name + "_" + exp_or_sim + "/txt_data/"

cur_slice = from_slice

# Maps/dicts containing the relative weights of each mixture with cur_slice as key
# (real) example:
#        weights_AM = {-190: 0.73387685, -188: 0.681326581759, ...}
#        weights_VR = {-190: 0.26612315, -188: 0.318673418241, ...}
weights_AM = collections.OrderedDict()
weights_VR = collections.OrderedDict()
means_AM = collections.OrderedDict()
means_VR = collections.OrderedDict()
sigma2s_AM = collections.OrderedDict()
sigma2s_VR = collections.OrderedDict() #sigma2s_AM and sigma2s_VR should be equal if we fit with tied covariance (s1=s2)
BIC_list = collections.OrderedDict()
BIC1_list = collections.OrderedDict()
BICdiff = collections.OrderedDict()

bootstrap_weights_AM = collections.OrderedDict()
bootstrap_weights_VR = collections.OrderedDict()
bootstrap_means_AM = collections.OrderedDict()
bootstrap_means_VR = collections.OrderedDict()
bootstrap_sigma2s_AM = collections.OrderedDict()
bootstrap_sigma2s_VR = collections.OrderedDict() #sigma2s_AM and sigma2s_VR should be equal if we fit with tied covariance (s1=s2)

# Format: {slice: weight_low} and {slice: weight_high} where weight_low and weight_high are the 15.87% and 84.13% percentiles (68.25% inside, 1 sigma)
weightsAM_low_errorbar = collections.OrderedDict()
weightsAM_high_errorbar = collections.OrderedDict()
weightsVR_low_errorbar = collections.OrderedDict()
weightsVR_high_errorbar = collections.OrderedDict()


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
clf_boot = mixture.GaussianMixture(
    n_components=2,
    covariance_type='tied',
    verbose=0,
    verbose_interval=10,
    random_state=random.SystemRandom().randrange(0, 4095),
    means_init=[[-17], [0]],
    #                              weights_init=[1 / 2, 1 / 2],
    init_params="kmeans",
    tol=1e-5,
    max_iter=100)
# TODO TODO finire bootstrap, in particolare scrivere funzione che fitta synth_dataset e tirarne fuori i valori
while (cur_slice < to_slice):
    if crystal_name == "STF45":
        slice_name = "Slices_" + str(cur_slice + 16) + "_" + str(
            cur_slice + 16 + deltaslice) + "_" + crystal_name
        data = np.loadtxt(data_folder + slice_name + ".txt")
    else:
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
    # Make the datapoints
    distribution = transform_histo_to_dataset(x,y)

    #pp.pprint(distribution)
    # Make the numpy array
    nd_distribution = np.array(distribution)

    # Create normalized histogram
    yn = y / np.sum(y)

    # Fit the two peaks
    # This reshape transforms (an np.array) [1,2,3] into [ [1], [2], [3] ]
    # Scikit wants a list of datapoints, here the datapoints coordinate are 1D, hence each one is a single-element list (of features/coordinates)
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






    # fit the "synth_dataset"s and fill the synthetic parameters array
    # Their structure is different from weights_AM etc:
    # Instead of weights_AM = {-190: 0.73387685, -188: 0.681326581759, ...}
    # we have bootstrap_weights_AM = {-190: [0.73387685, 0.754, 0.721, ...], -188: [0.681326581759. 0.692, 0.676, ...], ...}
    # where to each slice an array is assocaited with all the N values from the fitting of the N synth_dataset
    # TODO spostarli fuori? Per il momento sono leggermente inutili qua, come dict contengono un solo elemento d[cur_slice] = [array da numbootstrap elementi]
    # bootstrap_weights_AM = collections.OrderedDict()
    # bootstrap_weights_VR = collections.OrderedDict()
    # bootstrap_means_AM = collections.OrderedDict()
    # bootstrap_means_VR = collections.OrderedDict()
    # bootstrap_sigma2s_AM = collections.OrderedDict()
    # bootstrap_sigma2s_VR = collections.OrderedDict() #sigma2s_AM and sigma2s_VR should be equal if we fit with tied covariance (s1=s2)
    numbootstrap = 1000
    for i in range(0,numbootstrap):
        # synth_dataset = create_synth_dataset_from_histo(x, y, random.SystemRandom().randrange(0, 9223372036854775807))
        synth_dataset = create_synth_dataset_from_histo(x, y)
        np_synth_dataset = np.array(synth_dataset)
        clf_boot.fit(np_synth_dataset.reshape(-1, 1))

        # "Unflattened" variables
        b_m1, b_m2 = clf_boot.means_
        b_w1, b_w2 = clf_boot.weights_
        #    r_c1, r_c2 = clf.covariances_
        b_c1 = clf_boot.covariances_
        b_c2 = clf_boot.covariances_

        # Save the weights in the right array
        # Lower Y is the VR peak
        # setdefault return the value (which is actually a reference, important for our scope) if the key exists, otherwise it adds the key with the second argument as the value
        # Here we use it to make an empty list the first time and filling it
        if (m1 < m2):
            bootstrap_weights_VR.setdefault(cur_slice, []).append(b_w1)
            bootstrap_weights_AM.setdefault(cur_slice, []).append(b_w2)
            bootstrap_means_VR.setdefault(cur_slice, []).append(b_m1[0])
            bootstrap_means_AM.setdefault(cur_slice, []).append(b_m2[0])
            bootstrap_sigma2s_VR.setdefault(cur_slice, []).append(b_c1[0][0])
            bootstrap_sigma2s_AM.setdefault(cur_slice, []).append(b_c2[0][0])
        else:
            bootstrap_weights_VR.setdefault(cur_slice, []).append(b_w2)
            bootstrap_weights_AM.setdefault(cur_slice, []).append(b_w1)
            bootstrap_means_VR.setdefault(cur_slice, []).append(b_m2[0])
            bootstrap_means_AM.setdefault(cur_slice, []).append(b_m1[0])
            bootstrap_sigma2s_VR.setdefault(cur_slice, []).append(b_c2[0][0])
            bootstrap_sigma2s_AM.setdefault(cur_slice, []).append(b_c1[0][0])


    # Calculate the error intervals
    low_weightsAM_percentile = np.percentile(np.array(bootstrap_weights_AM[cur_slice]),15.87) # 15.87% = Gaussian -infinity -> -1sigma
    high_weightsAM_percentile = np.percentile(np.array(bootstrap_weights_AM[cur_slice]),84.13) # 84.13% = Gaussian -infinity -> 1sigma, -1s -> 1s = 68.25%
    weightsAM_low_errorbar[cur_slice] = low_weightsAM_percentile
    weightsAM_high_errorbar[cur_slice] = high_weightsAM_percentile

    low_weightsVR_percentile = np.percentile(np.array(bootstrap_weights_VR[cur_slice]),15.87) # 15.87% = Gaussian -infinity -> -1sigma
    high_weightsVR_percentile = np.percentile(np.array(bootstrap_weights_VR[cur_slice]),84.13) # 84.13% = Gaussian -infinity -> 1sigma, -1s -> 1s = 68.25%
    weightsVR_low_errorbar[cur_slice] = low_weightsVR_percentile
    weightsVR_high_errorbar[cur_slice] = high_weightsVR_percentile




    '''Plot the single slices fitted'''
    #fig = plt.figure(figsize = (5, 5))
    #plt.subplot(111)
    # gauss_test = 0.5*matplotlib.mlab.normpdf(x, -20, 8)
    double_freq_x = [0.5*xx for xx in range(int(2*x[0]), 2*int(x[-1]+1))]
    gauss1 = w1 * matplotlib.mlab.normpdf(double_freq_x, m1, np.sqrt(c1))
    gauss2 = w2 * matplotlib.mlab.normpdf(double_freq_x, m2, np.sqrt(c2))
    gauss_tot = gauss1 + gauss2


    plt.plot(
        x,
        yn,
        linestyle="dashed",
        drawstyle="steps-mid",
        label="Data",
        color='b')
    plt.plot(double_freq_x, gauss1, label="Gauss1", color='g')
    plt.plot(double_freq_x, gauss2, label="Gauss2", color='g')


    for i in range(0,numbootstrap):
        boot_tot = bootstrap_weights_VR[cur_slice][i] * matplotlib.mlab.normpdf(double_freq_x, bootstrap_means_VR[cur_slice][i], np.sqrt(bootstrap_sigma2s_VR[cur_slice][i])) + \
        bootstrap_weights_AM[cur_slice][i] * matplotlib.mlab.normpdf(double_freq_x, bootstrap_means_AM[cur_slice][i], np.sqrt(bootstrap_sigma2s_AM[cur_slice][i]))
        plt.plot(double_freq_x, boot_tot, color='DarkOrange',linewidth=0.8, alpha=0.1)

    plt.plot(double_freq_x, gauss_tot, label="Gauss_tot", color='r', linewidth=0.2)

    plt.legend()
    #plt.show()

    # plt.figure()  # New window, if needed.  No need to save it, as pyplot uses the concept of current figure
    # plt.plot(np.random.rand(10))
    # plt.show()

    figure_folder = "script/python_video/" + crystal_name + "_" + exp_or_sim + "/"
    #plt.savefig(figure_folder + "gaussian_fit.png", dpi=300)

    plt.savefig(figure_folder + slice_name + ".png", dpi=300)
    plt.clf()

    # Update cur_slice counter. Yeah maybe there's a more snakey way, don't care for now
    cur_slice = cur_slice + deltaslice






# np.percentile(np.array(sorted(bootstrap_weights_AM[174])),84.13)
# np.percentile(np.array(sorted(bootstrap_weights_AM[174])),84.13)

print("weights_AM:")
pp.pprint(weights_AM)
print("weights_VR:")
pp.pprint(weights_VR)

# Fit weights/proportions
from scipy.optimize import curve_fit
from scipy.special import erf


def erf_to_fit(xx, m, x0):
    return 0.5 * erf(m * (xx - x0)) + 0.5
    #return m*xx + x0
def parabola_to_fit(xx, a, b, c):
    return a*xx*xx + b*xx + c

x_AM = list(weights_AM.keys())
y_AM = list(weights_AM.values())
x_VR = list(weights_VR.keys())
y_VR = list(weights_VR.values())

y_meansAM = list(means_AM.values())
y_meansVR = list(means_VR.values())

y_sigmas = [np.sqrt(xx) for xx in sigma2s_VR.values()]

# weightsAM_low_yerr = [y_AM - yerrl for yerrl in weightsAM_low_errorbar.values()]
# weightsAM_high_yerr = [yerrh - y_AM for yerrh in weightsAM_high_errorbar.values()]
# weightsVR_low_yerr = [y_VR - yerrl for yerrl in weightsVR_low_errorbar.values()]
# weightsVR_high_yerr = [yerrh - y_VR for yerrh in weightsVR_high_errorbar.values()]

weightsAM_low_yerr = [weights_AM[i] - weightsAM_low_errorbar[i] for i in x_AM]
weightsAM_high_yerr = [weightsAM_high_errorbar[i] - weights_AM[i] for i in x_AM]
weightsVR_low_yerr = [weights_VR[i] - weightsVR_low_errorbar[i] for i in x_VR]
weightsVR_high_yerr = [weightsVR_high_errorbar[i] - weights_VR[i] for i in x_VR]

weightsAM_yerr = [weightsAM_low_yerr, weightsAM_high_yerr]
weightsVR_yerr = [weightsVR_low_yerr, weightsVR_high_yerr]

AM_parameters, AM_par_covars = curve_fit(
    erf_to_fit, x_AM, y_AM, p0=[1, (from_slice + to_slice) / 2])
Means_parameters, Means_params_covars = curve_fit(
    parabola_to_fit, x_AM, y_meansAM, p0=None)



#x_fitAM = np.linspace(-(interceptAM - 1) / slopeAM, -interceptAM / slopeAM, 100)
y_fitAM = [erf_to_fit(xx, AM_parameters[0], AM_parameters[1]) for xx in x_AM]
y_fitMeansAM = [parabola_to_fit(xx, Means_parameters[0], Means_parameters[1], Means_parameters[2]) for xx in x_AM]
y_fitVR = [1 - yy for yy in y_fitAM]

y_simgen = [erf_to_fit(xx, 1/(1.41421356*2*theta_c), theta_bending + theta_c) for xx in x_AM]


# http://matplotlib.org/api/markers_api.html marker numbers
if crystal_orientation == "R":
    #marker_AM = 6
    #marker_VR = 11
    marker_AM = 'x'
    marker_VR = 'x'
    or_sign = 1 # Orientation sign, to plot correctly the theta vertical bars
elif crystal_orientation == "L":
    #marker_AM = 6
    #marker_VR = 11
    marker_AM = '.'
    marker_VR = '.'
    or_sign = -1


#PLOT
# PLot theta_b, theta_b + theta_c, theta_b + 2theta_c
plt.clf()
plt.axvline(x=or_sign*(theta_bending + theta_c*0), linestyle="dashed", color='Chartreuse')  # TODO
plt.axvline(x=or_sign*(theta_bending + theta_c*0 + theta_c), linestyle="dashed")  #TODO
plt.axvline(x=or_sign*(theta_bending + theta_c*0 + 2*theta_c), linestyle="dashed")  #TODO

# Plot results
# plt.clf()
plt.errorbar(
    x_AM,
    y_AM,
    yerr=weightsAM_yerr,
    linestyle="dotted",
    marker=marker_AM,
    label="AM Data",
    color='g')
plt.errorbar(
    x_VR,
    y_VR,
    yerr=weightsVR_yerr,
    linestyle="dotted",
    marker=marker_VR,
    label="VR data",
    color='r')


#plt.plot(x_AM, y_fitAM, linestyle="solid", label="AM fit", color='Navy')
#plt.plot(x_VR, y_fitVR, linestyle="solid", label="VR fit", color='BlueViolet')
#plt.plot(x_AM, y_simgen, linestyle="dashed", label="Theoretical model", color='DarkOrange')



plt.title(crystal_name + "_" + exp_or_sim + ": weights")
plt.legend()
plt.show()

# Plot means
plt.figure()
plt.axhline(y=0, linestyle="dashed")
plt.axhline(y=theta_vr, linestyle="dashed")  #TODO
plt.plot(
    x_AM,
    y_meansAM,
    linestyle="dotted",
    marker="+",
    label="AM means",
    color='Crimson')
plt.plot(
    x_AM,
    y_meansVR,
    linestyle="dotted",
    marker="+",
    label="VR means",
    color='RoyalBlue')
plt.plot(
    x_AM,
    y_fitMeansAM,
    linestyle="solid",
    label="VR means",
    color='Aqua')
plt.title(crystal_name + "_" + exp_or_sim + ": means")
plt.legend()
plt.show()

# Plot sigmas
plt.figure()
plt.plot(
    x_AM,
    y_sigmas,
    linestyle="solid",
    label="Sigmas",
    color='BlueViolet')
plt.title(crystal_name + "_" + exp_or_sim + ": sigmas")
plt.legend()
plt.show()

print("Weights erf fit parameters: ")
print("m: ", AM_parameters[0], "-> c=1/m: ", 1 / AM_parameters[0])
print("x0: ", AM_parameters[1])

print("Covars matrix weights erf fit: ")
pp.pprint(AM_par_covars)
print()

print("Means parabola fit parameters: ")
print("a: ", Means_parameters[0])
print("b: ", Means_parameters[1])
print("c: ", Means_parameters[2])

s = input('--> ')

#fig.savefig('gaussian_fit.pdf')
