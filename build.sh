#!/bin/sh

# Build program
cd build
cmake ..
make
cd ..

# Run 
sudo ./build/app/widgets_opencv




