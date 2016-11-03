# How to run crystal simulations
Go to the right folder
 * `cd /afs/cern.ch/work/f/fforcher/` (I have aliased it as `wrk`)
 * `cd simulazioni_h8/`

Make a folder with the crystal name, eg *STF45* (`--no-preserve=mode` is needed because the original clean_input is write-protected, but we want the copy writable)
 * `mkdir STF45`
 * `cp -r prova/clean_input/ STF45/ --no-preserve=mode`
 * `cp -r prova/sixtrack_batch.sh STF45/ --no-preserve=mode`
 * `cd STF45/`

Check that the number of _packets_ (in the "file <<<EOF" .job) is the same as in clean_input/fort.3  

1. `clean_input/fort.3` (number of _packets_ here is _**"100"**_ in _**bold italic**_):

  >GEOM  
  >PRINTOUT  
  >NEXT  
  >TRACKING  
  >1 0 32 0 17 0 1  
  >1 1 0 0 0  
  >0 0 1 1 1 20000 2  
  >NEXT  
  >INITIAL COORDINATES  
  >2 0 0 1  
  >0.0  
  >0.0  
  >0.0  
  >0.0  
  >0.0  
  >0.0  
  >0.0  
  >.000001  
  >0.0  
  >0.0  
  >0.0  
  >0.0  
  >400000.0  
  >400000.0  
  >400000.0  
  >NEXT  
  >FLUCTUATION  
  >100000 1 7 3  
  >NEXT  
  >ITERATION ACCURACY  
  >50 0.10E-13 0.10E-14  
  >10 0.10E-09 0.10E-09  
  >10 0.10E-04 0.10E-05  
  >0.10E-07 0.10E-11 0.10E-09  
  >NEXT  
  >LINEAR OPTICS  
  >ELEMENT  0 1 1 3.5 3.5  
  >NEXT  
  >BEAM  
  >0.110E+12 2.89 2.89  0.0755E+00  1.0E-03 1 1 1  
  >NEXT  
  >SYNC  
  >4616 .00192 3.05 0 6911.5038 938.2796 1  
  >1 1  
  >NEXT  
  >COLLIMATION  
  >.TRUE.   
  >_**100**_ 400000  
  >4 3.7 0.05 0. 0. "distr_in.dat" 0.00 75.5  
  >.FALSE. 15.0 18.0 18.0 20.0 6.2 7.0 7.0 10.0 10.0 900.0 8.0 7.5 900.0  
  >8.3  8.3  8.3  8.3  8.3  8.3  8.3 8.3  900.0 900.0 6.0  
  >0 19789.0 20150.0 1 1  
  >0.0 0.4e-3 -0.4e-3 0.0 0.0 0.0 -1.0  
  >0.0 0.4e-3 -0.4e-3 0.0 0.0 0.0 -1.0  
  >1.0E-08 1.0E-08  
  >.FALSE. .FALSE. 0 .TRUE. CRY.A6L7.B1 .FALSE. .TRUE. .TRUE. .TRUE.  
  >0 0 0 0  
  >0E0  0E0 0E0 0E0  0E0  0E0  0E0  0E0  25  0E0  .FALSE.  
  >.FALSE. 5.958 .0015  
  >0 0 .FALSE. .FALSE.  
  >0 .0025 0.0 1.0E-3 1  
  >"SPS_horCrystal.dat" 1  
  >.TRUE. .FALSE. ScanCrysAngle-lb 12345 1 1.  
  >NEXT  
  >ENDE  

2. `sixtrack_batch.sh` (the "**100**" as indicated by `# <-- Here`):  

    ```sh
    #!/bin/bash

    # ALWAYS check:
    # beam 1 or 2
    # batch queue


    # LocalPWD : where on the mac should the files be copied. Does the directory exist?
    # post-processing programs to run
    # which output files should be copied back
    # should previous dirs run* be deleted


    PWD=`pwd`
    LocalPWD="/media/Daniele/STF45_H8_eff90_th/"

    beam=b1
    LIMIT=1000

    echo $PWD
    #scp -r $PWD/clean_input dmirarch@pcen33066:"${LocalPWD}"
    #scp $PWD/sixtrack_batch.sh dmirarch@pcen33066:"${LocalPWD}clean_input"


    #rm -r run*

    for ((a=1; a <= LIMIT ; a++))
      do
      index=$a
      zero=0
      while [ "${#index}" -lt "4" ]
        do
        index=$zero$index
      done
      mkdir run$index

      cat > run$index/SixTr$index.job << EOF
      #!/bin/bash
      cp $PWD/clean_input/* .
      cp $PWD/../H8_initial_distr.dat .

      ./make_distr H8_initial_distr.dat $a 100 # <---HERE

      ./SixTrack_dan_RELEASE_CRYSTAL > screenout

      cp cr_interaction.dat $PWD/run$index/

    #  scp -r $PWD/run${index} dmirarch@pcen33066:"${LocalPWD}"
    #  rm -r $PWD/run${index}
      exit
    EOF
    if [ -d "run$index" ]; then
        cd run$index
        chmod 777 SixTr$index.job
        bsub -q 1nh -R "rusage[pool=30000]" SixTr$index.job
        cd ..
    fi

    done
    ```  


* Set the number of jobs to start using the LIMITS variable in sixtrack_batch.sh
* Modify the file `clean_input/SPS-horCrystal.dat` (btw, those ~~"formats"~~ mess should **really** be reconsidered, guys. Like, at least, `field=value` input?):  

   >`# Database for cleaning insertion collimators`  
   >2  
   >`#`  
   >CRY.SPS.EXP    
   >cry.sps.exp  
   >0.0  
   >CRY-Si  
   >**_0.002_**  
   >0.0000000000000000  
   >0.000  
   >**_13.91_**  
   >0.002  
   >0.055  
   >0.0  
   >1  
   >0.0  
   >0.0  
   >137.62762  
   >90.64693  
   >`#`  
   >TCLA.SPSTAL     
   >tcla.spstal      
   >  20.0  
   >W  
   >0.6000000000000000  
   >0.0000000000000000  
   >0.0000000000000000  
   > 340.8223790000000000  
   >  46.4189010000000000

  The two marked numbers (here **_0.002_** and **_13.91_**) are respectively the lenght z of the crystal and the
  radius of curvature (both in _meters_).
  The other values are as follows, apparently starting to count the lines from the _fourth_ (after the `#`):  
   >  * **1-2** Crystal collimator name (which should match with the name specified in the fort.2    file)  
   >  * **3** Normalized aperture in sigma units.  
   >  * **4** Crystal material. The crystal routine of SixTrack allows to treat
   >    Silicon, Tungsten, Carbon or Germanium crystals.  
   >  * **5** Crystal total length [m], considered as the length of the curved surface of the    crystal.  
   >  * **6** Azimuthal angle rad of crystal (θtilt in picture 3.3/1). For the moment only purely    horizontal or vertical crystal are treated in the code (θtilt = 0 or θtilt = π/2 respectively).
   >  * **7** Transverse collimator gap offset [m].
   >  * **8** Bending radius [m].
   >  * **9-10** Transverse x-z dimension [m].
   >  * **11** Thickness of the amorphous layer at the crystal edge [m].
   >  * **12** Orientation of the crystal planes for silicon crystals (integer, can be 1 for the Si    110, or 2 for the Si 111). This parameter is ineffective in case of non-Si crystals.  
   >  * **13** (Wrong _"11"_ in the thesis table) Crystal tilt [rad].  
   >  * **14** (Wrong _"12"_ in the thesis table) Miscut angle [rad].  
   >  * **15-16** (Wrong _"13-14"_ in the thesis table) Design horizontal and vertical β-function [m] at the  collimator location  



* Run the script `./sixtrack_batch.sh`
* Wait for results to be processed, there will be `$LIMIT` folders named like `run0523` in which the most important file is `cr_interaction.dat`, which has this format:  

   ```sh
   #1=ipart 2=nturn 3=icoll 4=previous interaction 5=interaction      6=kick_x 7=kick_y 8=E_in 9=E_out 10=xp_in 11=yp_in 12=cr_ang
   101 1 1 -1 3 -1.407458291926671E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847 -3.035623000000000E-06 -5.586757000000000E-06 0.000000000000000E+00
   103 1 1 -1 2 1.364517736088041E-05 5.840654414214292E-06 400.0000000000000 399.9988751836152 -1.467048000000000E-05 7.107398000000000E-06 0.000000000000000E+00
   104 1 1 -1 3 -1.340521084495155E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847 -7.652649000000001E-07 -1.791178000000000E-05 0.000000000000000E+00
   105 1 1 -1 3 -1.423172636786826E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847 -8.704403000000000E-07 -2.047691000000000E-05 0.000000000000000E+00
   106 1 1 -1 6 -3.615577675191292E-05 4.733098115447369E-06 400.0000000000000 399.9989857961866 -3.070067000000000E-07 -3.483329000000000E-06 0.000000000000000E+00
   108 1 1 -1 1 7.335157409505806E-06 -4.405117384610454E-07 400.0000000000000 399.9988751827734 2.015113000000000E-05 1.454118000000000E-05 0.000000000000000E+00
   110 1 1 -1 3 -1.515446122448305E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847 3.520416000000000E-06 2.889906000000000E-06 0.000000000000000E+00
   111 1 1 -1 2 2.050969878214702E-05 -5.655064840065558E-06 400.0000000000000 399.9988751834110 -1.276804000000000E-05 2.963457000000000E-06 0.000000000000000E+00
   113 1 1 -1 3 -1.467361070152947E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847 4.789874000000000E-06 3.592512000000000E-06 0.000000000000000E+00
   114 1 1 -1 3 -1.374773039497378E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847 -1.462492000000000E-06 2.207778000000000E-05 0.000000000000000E+00
   115 1 1 -1 3 -1.362022184153167E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847 -6.658456000000000E-06 4.708235000000000E-06 0.000000000000000E+00
   116 1 1 -1 3 -1.529013645974008E-04 0.000000000000000E+00 400.0000000000000 399.9994375913847
   ```  
  The _first_ number is the particle ID, not important since they pass only once in the crystal. The _fourth_ is the previous interaction, `-1` since it's the first time they pass. The _fifth_ is the interaction type:
  * **-1**: *out*
  * **1**: *AM*
  * **2**: *VR*
  * **3**: *CH*
  * **4**: *VC*
  * **5**: *absorbed*
  * **6**: *DC*
  * **7**: *pne*
  * **8**: *ppe*
  * **9**: *diff*
  * **10**: *ruth*
