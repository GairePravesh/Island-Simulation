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

  CreateShaders();
  CreateVBO();
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
  glutCloseFunc(Cleanup);
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
  glDrawArrays(GL_TRIANGLES, 0, 3);
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

void Cleanup(void)
{
  DestroyShaders();
  DestroyVBO();
}

void CreateVBO(void)
{
  GLfloat Vertices[] = {
    -0.8f, -0.8f, 0.0f, 1.0f,
     0.0f,  0.8f, 0.0f, 1.0f,
     0.8f, -0.8f, 0.0f, 1.0f
  };

  GLfloat Colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
  };

  GLenum ErrorCheckValue = glGetError();

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);

  glGenBuffers(1, &VboId);
  glBindBuffer(GL_ARRAY_BUFFER, VboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &ColorBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not create a VBO: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
 }

 void DestroyVBO(void)
{
  GLenum ErrorCheckValue = glGetError();

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &ColorBufferId);
  glDeleteBuffers(1, &VboId);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VaoId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not destroy the VBO: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}

void CreateShaders(void)
{
  GLenum ErrorCheckValue = glGetError();

  VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
  glCompileShader(VertexShaderId);

  FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
  glCompileShader(FragmentShaderId);

  ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
  glLinkProgram(ProgramId);
  glUseProgram(ProgramId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not create the shaders: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}

void DestroyShaders(void)
{
  GLenum ErrorCheckValue = glGetError();

  glUseProgram(0);

  glDetachShader(ProgramId, VertexShaderId);
  glDetachShader(ProgramId, FragmentShaderId);

  glDeleteShader(FragmentShaderId);
  glDeleteShader(VertexShaderId);

  glDeleteProgram(ProgramId);

  ErrorCheckValue = glGetError();
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not destroy the shaders: %s \n",
      gluErrorString(ErrorCheckValue)
    );

    exit(-1);
  }
}