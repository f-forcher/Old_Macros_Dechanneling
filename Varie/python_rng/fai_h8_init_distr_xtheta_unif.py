#!/usr/bin/python3

import random

x = []  # x [m]
tx = [] # ThetaX [rad]
y = []  # y [m]
ty = [] # ThetaY [rad]
displacement = [] # displacement [rad]
energy = [] # energy [MeV]



with open('H8_initial_distr.dat') as f:
    for line in f:
        data = line.split()
        x.append(float(data[0]))
        tx.append(float(data[1]))
        y.append(float(data[2]))
        ty.append(float(data[3]))
        displacement.append(float(data[4]))
        energy.append(float(data[5]))

print("Righe lette: ", len(x))


for i in range(0,10):
    print(random.randint(5,10))


f1=open('./H8_initial_distr_xp_uniforme.dat', 'w')
for i in range(0, len(x)):
    random_tx = random.uniform(-200,50)
    print(x[i], random_tx*1e-6, y[i], ty[i], displacement[i], energy[i], file=f1)

