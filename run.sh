#!/usr/bin/env bash


gcc -o prog global.c main.c -I /usr/include/X11 -L /usr/X11/lib -lX11 -lpthread  &&
./prog
