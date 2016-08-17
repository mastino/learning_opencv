#!/bin/bash

# example build script for opencv and extra modules
# put in [path to opencv source]/release/

cmake -D CMAKE_BUILD_TYPE=Release -D OPENCV_EXTRA_MODULES_PATH=/home/pi/opencv_contrib/modules/ -D CMAKE_INSTALL_PREFIX=/usr/ ..
sudo make -j4
sudo make install

