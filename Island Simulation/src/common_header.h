#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <ctime>
#include <windows.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <sstream>
#include <queue>
#include <map>
#include <set>

using namespace std;

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Some useful defines

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()



extern unsigned int screen_width;
extern unsigned int screen_height;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern float currentFrame;
extern float deltaTime;
extern float lastFrame;
extern float fFrameInterval;
extern float fAngleOfDarkness;


#endif