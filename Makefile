# OBJS specifies which files to compile as part of the project
OBJS = main.c glad.c

# COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c11 -Wall -Wextra $(shell pkg-config --cflags glfw3)

#LIBS specifies the additional libraries
LIBS = -L"./libs" $(shell pkg-config --libs glfw3) -ldl -lm -lidl

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = -o ./out/game

all : $(OBJS)
	gcc $(OBJS) $(COMPILER_FLAGS) $(LIBS) $(OBJ_NAME)