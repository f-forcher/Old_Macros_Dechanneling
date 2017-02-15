#!/bin/python3
from sklearn import mixture
import sys
import matplotlib.pyplot as plt
plt.ion()
import matplotlib.mlab
import numpy as np
from pylab import *
import random

import pprint
pp = pprint.PrettyPrinter(indent=4)





# "Main"
crystal_name = sys.argv[1]
exp_or_sim = sys.argv[2]
from_slice = int(sys.argv[3]) # [murad] Has to be an existing slice, for now
to_slice = int(sys.argv[4])
deltaslice = 2

data_folder = "ForFrancesco/" + crystal_name +"_"+ exp_or_sim + "/txt_data/"

cur_slice = from_slice
while (cur_slice < to_slice):

    slice_name = "Slices_" + str(cur_slice) + "_" + str(cur_slice + deltaslice) + "_" + crystal_name
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
        for j in range(int(np.round(altezza_col) ) ):
            distribution.append(x[i])

    #pp.pprint(distribution)
    # Make the numpy array
    nd_distribution = np.array(distribution)

    # Create normalized histogram
    yn = y / np.sum(y)




    clf = mixture.GaussianMixture(n_components=2, covariance_type='full',verbose=2,verbose_interval=10,
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
    plt.plot(x,gauss_tot,label = "Gauss_tot", color='r')

    plt.legend()
    #plt.show()

    # plt.figure()  # New window, if needed.  No need to save it, as pyplot uses the concept of current figure
    # plt.plot(np.random.rand(10))
    # plt.show()

    figure_folder = "script/python_video/" + crystal_name +"_"+ exp_or_sim+"/"
    #plt.savefig(figure_folder + "gaussian_fit.png", dpi=300)

    plt.savefig(figure_folder + slice_name + ".png", dpi=300)
    plt.clf()

    # Update cur_slice counter. Yeah maybe there's a more snakey way, don't care for now
    cur_slice = cur_slice + deltaslice


s = input('--> ')


#fig.savefig('gaussian_fit.pdf')
