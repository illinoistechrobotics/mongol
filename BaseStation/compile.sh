#!/bin/bash

echo "Cleaning..."
rm BaseStation

echo "Compiling..."
gcc -g `sdl-config --cflags` `pkg-config --cflags gtk+-3.0` -o BaseStation *.c `sdl-config --libs` `pkg-config --libs gtk+-3.0`

echo "Done."
