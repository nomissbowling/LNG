/*
  LNGframe.cpp
*/

#include "LNGframe.h"

using namespace std;

LNGsize const LNGframe::default_size(640, 480);
LNGpoint const LNGframe::default_pos(320, 240);

LNGframe *LNGdispatcher::frame = 0;

LNGframe::LNGframe() : dispatcher(0), fps(0)
{
  if(!dispatcher) dispatcher = new LNGdispatcher(this);
  if(!fps) fps = new LNGclock();
}

LNGframe::~LNGframe()
{
  if(fps){ delete fps; fps = 0; }
  if(dispatcher){ delete dispatcher; dispatcher = 0; }
}

void LNGframe::InitFrame(int *ac, char **av, std::string &title,
  LNGsize size, LNGpoint pos, GLuint mode)
{
#ifdef _DEBUG
  cout << "ac: " << *ac;
  for(int i = 0; i < *ac; i++) cout << ", " << av[i];
  cout << endl;
  cout << "size: " << size.w << ',' << size.h << endl;
  cout << "pos: " << pos.x << ',' << pos.y << endl;
#endif
  glutInit(ac, av);
  glutInitDisplayMode(mode);
  glutInitWindowSize(size.w, size.h);
  glutInitWindowPosition(pos.x, pos.y);
  glutCreateWindow(title.c_str());
  glutTimerFunc(0, dispatcher->Timer, 0);
  glutIdleFunc(dispatcher->Idle);
  glutDisplayFunc(dispatcher->Display);
  glutReshapeFunc(dispatcher->Reshape);
  glutKeyboardFunc(dispatcher->KeyPress);
  InitGL();
  glutMainLoop();
}

void LNGframe::LoadTextures(void)
{
}

void LNGframe::InitGL(void)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST); // glutInitDisplayMode() GLUT_DEPTH
  LoadTextures();
}

void LNGframe::Timer(int dt)
{
}

void LNGframe::Idle(void)
{
}

void LNGframe::Update(void)
{
}

void LNGframe::ChangeAngle(void)
{
}

void LNGframe::ChangeView(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  ChangeAngle();
  Display();
}

void LNGframe::DisplayBefore(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LNGframe::DisplayDraw(void)
{
  glDisable(GL_TEXTURE_2D);
  glColor4f(1.0, 0.0, 0.0, 1.0);
  glRasterPos3f(0.0, 0.0, 0.0);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '*');
  glEnable(GL_TEXTURE_2D);
}

void LNGframe::DisplayAfter(void)
{
  // glFlush(); // glutInitDisplayMode() GLUT_SINGLE ?
  glutSwapBuffers(); // glutInitDisplayMode() GLUT_DOUBLE (double buffering)
}

void LNGframe::Display(void)
{
  DisplayBefore();
  DisplayDraw();
  DisplayAfter();
}

void LNGframe::Perspective(int w, int h)
{
}

void LNGframe::Reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  Perspective(w, h);
  ChangeView();
}

void LNGframe::KeyPress(unsigned char key, int x, int y)
{
  if(key == 27){ // ESC
    glutDestroyWindow(glutGetWindow());
    exit(0);
  }
}
