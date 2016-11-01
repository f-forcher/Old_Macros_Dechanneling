# Analysis of fast nuclear dechanneling in highly bent crystals
<!--![bg 80%](channeling_small.gif)-->

---

# Introduction
<!-- page_number: true -->
<!-- footer: Nuclear dechanneling analysis - Introduction -->
In order to write a better simulation routine for highly bent (low radius of curvature) crystals, we need to analyze experimental data to discover the relationship between the curvature and the dechanneling of particles out of the crystal planes.

---

# Theory
<!-- footer: Nuclear dechanneling analysis - Theory-->

## Electronic dechanneling
Dechanneling caused by scattering with electrons can be approximately described by diffusion theory as an exponential
$$ N(z)\  \propto \ e^{-\frac{z}{L_D} } $$
Where N is the dechanneled fraction of the particles, z is the depth distance inside the crystal and $L_D$ is called _**dechanneling length**_.

---

An analytical approximation of $L_D$ is the following:
$$L_D = \frac{256}{\pi^2} \frac{pv}{ln(2 m_e c^2 \gamma I^{-1})-1} \frac{a_{TF} d_p}{Z_i r_e m_e c^2}$$
Where 
* $p$, $v$, $\gamma$ and $Z_i$ are the *incoming particle*'s **momentum**, **speed**, **gamma** and **charge number**.
* $c$, $r_e$, $m_e$ are the **speed of light**, the **mass** and the **classical radius** of the *electron*.
* $a_{TF}$, $d_p$, and $I$ are respectively the **Thomas-Fermi radius**, the **interplanar distance** and the **mean excitation energy** (properties of the crystal's *material* and channeling *plane*).

---
## Limitations of this model
The simple exponential model can be used approximatively from  
$$ z > 0.2 \, L_D$$  
For $400\ GeV/c$ protons in Si crystals, $L_D $ is roughly $22\ cm$, but the crystals used were at most a couple of *millimeters* long, so electronic scattering is not the main cause of particle dechanneling in them. Then the dechanneling length depends upon the initial conditions of the beam hitting the crystal, and we have to account for fast nuclear dechanneling.

---


## Crystal bending

---

# Data 
<!-- footer: Nuclear dechanneling analysis - Data -->
## Types of file:

* .root
* .dat