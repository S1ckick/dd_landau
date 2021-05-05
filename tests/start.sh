#!/bin/bash
racket spacecraft.dau -c -td
gcc -Wall -c main.c
gcc -Wall -c spacecraft.c
gcc -Wall -c dd.c
gcc -o end main.o spacecraft.o dd.o
./end
