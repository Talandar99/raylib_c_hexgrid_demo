#!/bin/bash

rm a.out
cc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./a.out
rm a.out
