# OBJS specifies which files to compile as part of the project
OBJS = main.c glad.c

# CC specifies which compiler we're using
CC = gcc

# COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = $(shell pkg-config --cflags glfw3) -std=c11 -Wall -Wextra 

#LIBS specifies the additional libraries
LIBS = $(shell pkg-config --libs glfw3) -ldl -lm

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = -o ./out/game

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LIBS) $(OBJ_NAME)