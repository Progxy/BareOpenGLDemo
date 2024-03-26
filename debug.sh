#!/bin/bash

set -xe
env PKG_CONFIG_PATH=/usr/local/lib/glfw/src make debug
export LD_LIBRARY_PATH=:~/Informatica/OpenGL/libs
gdb ./out/game
