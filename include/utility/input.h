#ifndef _INPUT_H_
#define _INPUT_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"
#endif //_STDLIB_DEF_

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, TRUE);
    }
}

#endif //_INPUT_H_