#include <iostream>
#include "window.cpp"

int main(int argc, char* argv[])
{
  Initialize(argc, argv);
  
  // draw
  
  glutMainLoop();
  
  exit(EXIT_SUCCESS); 
}

