#! /bin/bash
sudo cp $1/libstdc++.so.6 $1/libstdc++.so.6_backup 
sudo cp ./libstdc++.so.6.0.22 $1
cd $1
sudo rm libstdc++.so.6 
sudo ln -s libstdc++.so.6.0.22 libstdc++.so.6