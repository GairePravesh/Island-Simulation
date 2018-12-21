#include "window.hpp"

void Initialize(int argc, char* argv[])
{
  GLenum GlewInitResult;

  InitWindow(argc, argv);
  
  GlewInitResult = glewInit();
  if (GLEW_OK != GlewInitResult) {
    std::cout<<"ERROR: \n"<<glewGetErrorString(GlewInitResult)<<std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout<<"OpenGL Version: "<<glGetString(GL_VERSION)<<std::endl;

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void InitWindow(int argc, char* argv[])
{
  glutInit(&argc, argv);
  
  glutInitContextVersion(4, 0);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  
  glutInitWindowSize(CurrentWidth, CurrentHeight);

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

  if(WindowHandle < 1) {
    std::cout<<"ERROR: Could not create a new rendering window.\n"<<std::endl;
    exit(EXIT_FAILURE);
  }

  glutReshapeFunc(ResizeFunction);
  glutDisplayFunc(RenderFunction);
  glutIdleFunc(IdleFunction);
  glutTimerFunc(0, TimerFunction, 0); 
}

void ResizeFunction(int Width, int Height)
{
  CurrentWidth = Width;
  CurrentHeight = Height;
  glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void)
{
  ++FrameCount;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutSwapBuffers();
  glutPostRedisplay();
}

void IdleFunction(void)
{
  glutPostRedisplay();
}

void TimerFunction(int Value)
{
  if (Value) {
    std::cout<<FrameCount<<" Frames Per Second"<<std::endl;
  }
  
  FrameCount = 0;
  glutTimerFunc(1000, TimerFunction, 1);
}