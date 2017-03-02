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
import math #erf
import collections

import pprint
pp = pprint.PrettyPrinter(indent=4)

# "Main"
crystal_name = sys.argv[1]
exp_or_sim = sys.argv[2]
from_slice = int(sys.argv[3])  # [murad] Has to be an existing slice, for now
to_slice = int(sys.argv[4])
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
BIC_list = collections.OrderedDict()
BIC1_list = collections.OrderedDict()
BICdiff = collections.OrderedDict()

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

clf1 = mixture.GaussianMixture(
    n_components=1,
    covariance_type='tied',
    verbose=2,
    verbose_interval=10,
    random_state=random.SystemRandom().randrange(0, 4095),
    init_params="kmeans",
    tol=1e-5,
    max_iter=1000)
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

    # Fit the two peaks and save the BIC
    clf.fit(nd_distribution.reshape(-1, 1))
    clf1.fit(nd_distribution.reshape(-1, 1))
    BIC_list[cur_slice] = clf.bic(nd_distribution.reshape(-1, 1))
    BIC1_list[cur_slice] = clf1.bic(nd_distribution.reshape(-1, 1))

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
    # AM_mean = 0  # [murad], should be close to zero, TODO maybe take the mean from the first slice
    # lower Y slice is in VR ___---
    if (m1 < m2):
        weights_VR[cur_slice] = w1
        weights_AM[cur_slice] = w2
        means_VR[cur_slice] = m1
        means_AM[cur_slice] = m2
    else:
        weights_VR[cur_slice] = w2
        weights_AM[cur_slice] = w1
        means_VR[cur_slice] = m2
        means_AM[cur_slice] = m1

    #fig = plt.figure(figsize = (5, 5))
    #plt.subplot(111)
    # gauss_test = 0.5*matplotlib.mlab.normpdf(x, -20, 8)
    gauss1 = w1 * matplotlib.mlab.normpdf(x, m1, np.sqrt(c1))
    gauss2 = w2 * matplotlib.mlab.normpdf(x, m2, np.sqrt(c2))
    gauss_tot = gauss1 + gauss2

    plt.plot(x, yn, linestyle="dashed", drawstyle="steps-mid", label="Data", color='b')
    plt.plot(x, gauss1, label="Gauss1", color='g')
    plt.plot(x, gauss2, label="Gauss2", color='g')
    plt.plot(x, gauss_tot, label="Gauss_tot", color='r')

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

print("weights_AM:")
pp.pprint(weights_AM)
print("weights_VR:")
pp.pprint(weights_VR)

# print("BICs:")
# pp.pprint(BIC_list)
# print("BIC1s:")
# pp.pprint(BIC1_list)
# BICdiff = collections.OrderedDict()
# for k in BIC_list.keys():
#     BICdiff[k] = BIC_list[k] - BIC1_list[k]
# print("BIC diffs:")
# pp.pprint(BICdiff)




# Fit weights/proportions
from scipy.optimize import curve_fit
from scipy.special import erf
def erf_to_fit(xx, m, x0):
    return 0.5*erf(m*(xx - x0)) + 0.5
    #return m*xx + x0

x_AM = list(weights_AM.keys())
y_AM = list(weights_AM.values())
x_VR = list(weights_VR.keys())
y_VR = list(weights_VR.values())

y_meansAM = list(means_AM.values())
y_meansVR = list(means_VR.values())


AM_parameters, AM_par_covars = curve_fit(erf_to_fit, x_AM, y_AM, p0=[1,(from_slice + to_slice)/2])


#x_fitAM = np.linspace(-(interceptAM - 1) / slopeAM, -interceptAM / slopeAM, 100)
y_fitAM = [erf_to_fit(xx, AM_parameters[0], AM_parameters[1]) for xx in x_AM]
y_fitVR = [1-yy for yy in y_fitAM]







# http://matplotlib.org/api/markers_api.html marker numbers
# TODO perche' differenziare per orientation??
if crystal_orientation == "R":
    marker_AM = 6
    marker_VR = 11
elif crystal_orientation == "L":
    marker_AM = 11
    marker_VR = 6


# Plot results
# TODO generalize to other crystals
plt.clf()
plt.plot(x_AM, y_AM, linestyle="dotted", marker=marker_AM, label="AM Data", color='g')
plt.plot(x_VR, y_VR, linestyle="dotted", marker=marker_VR, label="VR data", color='r')

plt.plot(x_AM, y_fitAM, linestyle="solid", label="AM fit", color='Navy')
plt.plot(x_VR, y_fitVR, linestyle="solid", label="VR fit", color='BlueViolet')

#plt.axvline(x=144.667+ 9.5, linestyle="dashed") # TODO
#plt.axvline(x=144.667 + 9.5+ 9.5, linestyle="dashed") #TODO
#plt.axvline(x=144.667 + 2*9.5+ 9.5, linestyle="dashed") #TODO


plt.title(crystal_name + " " + exp_or_sim)
plt.legend()
plt.show()


# Plot means
plt.figure()
#plt.axhline(y=0, linestyle="dashed")
#splt.axhline(y=-12.53, linestyle="dashed") #TODO
plt.plot(x_AM, y_meansAM, linestyle="dotted", marker="+", label="AM means", color='Crimson')
plt.plot(x_AM, y_meansVR, linestyle="dotted", marker="+", label="VR means", color='RoyalBlue')
plt.title(crystal_name + " " + exp_or_sim)
plt.legend()
plt.show()



print("Covars matrix")
pp.pprint(AM_par_covars)
print()
print("m: ", AM_parameters[0], "-> c=1/m: ", 1/AM_parameters[0])
print("x0: ", AM_parameters[1])
s = input('--> ')

#fig.savefig('gaussian_fit.pdf')