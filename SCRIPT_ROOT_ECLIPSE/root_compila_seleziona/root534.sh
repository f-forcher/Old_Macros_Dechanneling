#!/bin/sh

echo "Selecting root 5.34.36!\n"
echo "Setting compiler to gcc 4.9"
echo "Password: "



# Pure se usiamo clang, rimettiamo gcc a posto
# Sono disponibili 2 scelte per l'alternativa g++ (che fornisce /usr/bin/g++).
# 
#   Selezione    Percorso          Priorità  Stato
# ------------------------------------------------------------
# * 0            /usr/bin/g++-5     20        modalità automatica
#   1            /usr/bin/g++-4.9   10        modalità manuale
#   2            /usr/bin/g++-5     20        modalità manuale
# 
# Press <enter> to keep the current choice[*], or type selection number: 0
sudo update-alternatives --set gcc "/usr/bin/gcc-4.9"
sudo update-alternatives --set g++ "/usr/bin/g++-4.9"


# Sono disponibili 2 scelte per l'alternativa c++ (che fornisce /usr/bin/c++).
# 
#   Selezione    Percorso          Priorità  Stato
# ------------------------------------------------------------
# * 0            /usr/bin/clang++   40        modalità automatica
#   1            /usr/bin/clang++   40        modalità manuale
#   2            /usr/bin/g++       30        modalità manuale
# 
# Press <enter> to keep the current choice[*], or type selection number: %                                         
sudo update-alternatives --set cc "/usr/bin/gcc"
sudo update-alternatives --set c++ "/usr/bin/g++"


echo
g++ --version
echo

echo "cd ~/ROOT/v5-34-36/; source bin/thisroot.sh"

cd $HOME/ROOT/v5-34-36 
source bin/thisroot.sh
cd
