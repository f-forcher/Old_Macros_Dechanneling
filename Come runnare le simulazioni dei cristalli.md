# How to run crystal simulations
Go to the right folder
 * `./wrk` or `cd /afs/cern.ch/work/f/fforcher/`
 * `cd simulazioni_h8/`

Make a folder with the crystal name, eg *STF45* (`--no-preserve=mode` is needed because the original clean_input is write-protected, but we want the copy writable)
 * `mkdir STF45`
 * `cp -r prova/clean_input/ STF45/ --no-preserve=mode`
 * `cp -r prova/sixtrack_batch.sh STF45/ --no-preserve=mode`
 * `cd STF45/`

 Check that the number of _packets?_ (in the "file <<<EOF" .job) is the same as in clean_input/fort.3  
   * clean_input/fort.3 (number of _packets?_ "**100**" in **bold**):
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
    >**100** 400000  
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

   * sixtrack_batch.sh (the "**100**" as indicated by `# <-- Here`):  
   
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


  cp non_writable ./copy --no-preserve=mode
