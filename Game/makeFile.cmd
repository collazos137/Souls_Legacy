@echo off
gcc -c main.c
gcc -c manejoArchivos.c
gcc -c batalla.c
gcc -c menu.c
gcc -c usosVariados.c
gcc -c lista.c
gcc manejoArchivos.o main.o batalla.o menu.o usosVariados.o lista.o -o souls
.\eraseO.cmd