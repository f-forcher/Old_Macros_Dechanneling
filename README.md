# Old_Macros_Dechanneling
Old ROOT macros for dechanneling, wich I`m modifying to analyze (old and new) crystals

# How to use
###Installation/Preliminary operations
1. Install/compile the various versions of ROOT and put them in ~/ROOT/vXX-XX-XX (eg "~/ROOT/v5-34-36").
2. Put the   `SCRIPT_ROOT_ECLIPSE/Documents/root_varie` in `~/Documents` (or modify the scripts).
   
   The reason here is that at the moment, ROOT 5 and ROOT 6 are not compatible. Even worse, gcc 4 and 5's ABis are not
   compatible and root 5 "prefers" g++ 4.x and root 6 g++ 5.x (or clang). So the scripts use update-alternatives
   to select the appropriate compiler (you need to install them both obviously), and they need to use sudo.
   
3. Put the aliases for the shell (bash, zsh, etc) in the corresponding file and source them.
3. Put Eclipse (or modify the scripts, obviously...) in `~/bin/eclipse`
4. Put the scripts to run eclipse (eg `~/SCRIPT_ROOT_ECLIPSE/eclipse_root534.sh`) for example on the Desktop

###Compile/run
1. Open a terminal and select the root to use with the aliases (eg `root_534_select`) or soucing the scripts in `~/Documents/root_varie`
2. Open eclipse using the script corresponding the selected ROOT
* From terminal you can compile (assuming as usual you are in the folder) for example with Debug congfig with `cd ./Debug; make` and then you go back up `cd ..` and execute `./Debug/Old_Macros_Dechanelling`
* Alternatively you can load and execute the macro main_macro() (the equivalent of main) from root, with for example

#Usage
The file .dat to analyze (not in github as they are too big to use) are
