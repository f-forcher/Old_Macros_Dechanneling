#!/bin/python3
from sklearn import mixture
import matplotlib.pyplot as plt

import matplotlib.mlab
import numpy as np
from pylab import *
import random

import pprint
pp = pprint.PrettyPrinter(indent=4)

def fit_mixture(data, ncomp, doplot=False):
    clf = mixture.GaussianMixture(n_components=2, covariance_type='full',verbose=2,verbose_interval=2,
    #                              means_init=[[-17],[0]], weights_init=[1 / 2, 1 / 2],
    #                              random_state=random.SystemRandom().randrange(0,4095),
                                  init_params="kmeans",
                                  tol=1e-5 )
    clf.fit(data.reshape(-1, 1))


#data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-170_-169_ST101.txt')
#data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-186_-185_ST101.txt')
data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-142_-141_ST101.txt')

x = data[:, 0]
y = data[:, 1]

distribution = []
# Create (deterministic) samples from the histogram
for i in range(len(y)):
    posizione_col = x[i]
    altezza_col = y[i]
    for j in range(int(np.round(altezza_col) ) ):
        distribution.append(x[i])

#pp.pprint(distribution)
nd_distribution = np.array(distribution)

yn = y / np.sum(y)
print("yn.sum(): ", yn.sum())



clf = mixture.GaussianMixture(n_components=2, covariance_type='full',verbose=2,verbose_interval=2,
#                              means_init=[[-17],[0]], weights_init=[1 / 2, 1 / 2],
#                              random_state=random.SystemRandom().randrange(0,4095),
                              init_params="kmeans",
                              tol=1e-5 )
clf.fit(nd_distribution.reshape(-1, 1))


# "Unflattened" variables
r_m1, r_m2 = clf.means_
w1, w2 = clf.weights_
r_c1, r_c2 = clf.covariances_
print("means_: ", clf.means_)
print("weights_: ", clf.weights_)
print("covariances_: ", clf.covariances_)


print("First peak: ", r_m1[0], "Second peak:", r_m2[0])
print("w1: ", w1, "w2:", w2)
print("c1: ", r_c1[0][0], "c2:", r_c2[0][0])

# Flatten variables
# m1, m2 = r_m1[0], r_m2[0]
# c1,  c2 = r_c1[0][0], r_c2[0][0]

# TODO perche se le flatto non funziona?
m1, m2 = r_m1, r_m2
c1, c2 = r_c1, r_c2


#fig = plt.figure(figsize = (5, 5))
#plt.subplot(111)
# gauss_test = 0.5*matplotlib.mlab.normpdf(x, -20, 8)
gauss1 = w1*matplotlib.mlab.normpdf(x,m1,np.sqrt(c1))[0]
gauss2 = w2*matplotlib.mlab.normpdf(x,m2,np.sqrt(c2))[0]
gauss_tot = gauss1 + gauss2


plt.plot(x,yn,"-.",label = "Data", color='b')
plt.plot(x,gauss1,label = "Gauss1", color='g')
plt.plot(x,gauss2,label = "Gauss2", color='g')
plt.plot(x,gauss_tot,label = "Gauss_tot", color="r")

plt.legend()
plt.show()

plt.plot(np.random.rand(10))
plt.show()

#fig.savefig('gaussian_fit.pdf')
