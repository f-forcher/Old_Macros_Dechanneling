#!/bin/zsh

cd ~/ROOT/v6-06-08
source ./bin/thisroot.sh
cd

# TODO switchare compiler, ma richiederebbe la password di root (il superuser...)
notify-send "May need to set the compiler with root_606_select";
notify-send "$(c++ --version)"

# TODO fare simbolic link a /usr/local/include
# echo "Linkando ~/ROOT/v5-34-36 a /usr/local/include/ROOT"
# cd /usr/local/include
# sudo rm -f ROOT
# sudo ln -s ~/ROOT/v5-34-36/include ROOT
# ls ROOT
# cd

~/bin/eclipse/eclipse
