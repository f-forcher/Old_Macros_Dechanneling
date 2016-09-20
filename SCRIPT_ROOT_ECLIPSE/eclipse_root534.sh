#!/bin/zsh

# Avvia eclipse con l'ambiente settato giusto

cd ~/ROOT/v5-34-36
source ./bin/thisroot.sh
cd

# TODO fare simbolic link a /usr/local/include
# echo "Linkando ~/ROOT/v5-34-36 a /usr/local/include/ROOT"
# cd /usr/local/include
# sudo rm -f ROOT
# sudo ln -s ~/ROOT/v5-34-36/include ROOT
# ls ROOT
# cd

~/bin/eclipse/eclipse
