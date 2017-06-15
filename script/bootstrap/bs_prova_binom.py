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



# Binomial data
n, p = 100, 30/100
theoretical_sigma = np.sqrt(n*p*(1-p))
#sample = np.random.binomial(n, p)
# 'r'ed or 'w'hite balls extracted from a 30% very large urn
sample = ['r' if np.random.random() < p else 'w' for ii in range(n)]

# Count the red balls
nreds_sample = sum( (1 if b=='r' else 0) for b in sample)

#TODO Bootstrap samples
bs = np.random.random_integers(0,n,n)
bs_sample1 = [sample[ii] for ii in bs]
nreds_bs_sample1 = sum(1 if b=='r' else 0 for b in bs_sample1)
