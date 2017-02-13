#!/bin/python3
from sklearn import mixture
import matplotlib.pyplot as plt
import matplotlib.mlab
import numpy as np
from pylab import *

import pprint
pp = pprint.PrettyPrinter(indent=4)

data = np.loadtxt('ForFrancesco/ST101_exp/txt_data/Slices_-170_-169_ST101.txt')
x = data[:, 0]
y = data[:, 1]

print(data)

clf = mixture.GaussianMixture(n_components=2, covariance_type='full',verbose=2,verbose_interval=2)
clf.fit(y.reshape(-1, 1))
# "Unflattened" variables
r_m1, r_m2 = clf.means_
w1, w2 = clf.weights_
r_c1, r_c2 = clf.covariances_
pp.pprint(clf.means_)
pp.pprint(clf.weights_)
pp.pprint(clf.covariances_)


print("First peak: ", r_m1[0], "Second peak:", r_m2[0])
print("w1: ", w1, "w2:", w2)
print("c1: ", r_c1[0][0], "c2:", r_c2[0][0])

# Flatten variables
# m1, m2 = r_m1[0], r_m2[0]
# m1, m2 = r_c1[0][0], r_c2[0][0]

# TODO perche se le flatto non funziona?
m1, m2 = r_m1, r_m2
c1, c2 = r_c1, r_c2


#fig = plt.figure(figsize = (5, 5))
#plt.subplot(111)
gauss1 = w1*matplotlib.mlab.normpdf(x,m1,np.sqrt(c1))[0]
gauss2 = w2*matplotlib.mlab.normpdf(x,m2,np.sqrt(c2))[0]
plt.plot(range(0,len(x)),y,"-.",label = "Data", color='r')
plt.plot(range(0,len(x)),gauss1,label = "Gauss1", color='g')
plt.plot(range(0,len(x)),gauss2,label = "Gauss2", color='b')
plt.legend()
plt.show()
#fig.savefig('gaussian_fit.pdf')
