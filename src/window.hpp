#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "constants.hpp"

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);