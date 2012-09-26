/*
  testLNG2D.cpp

  > cl testLNG2D.cpp LNG2Dframe.cpp \
    LNGframe.cpp LNGclock.cpp glut32.lib glpng.lib -EHsc -I. \
    -link -LIBPATH:. -SUBSYSTEM:console -NODEFAULTLIB:libc

  -EHsc destroy local objects on catch any exception
  -NODEFAULTLIB:libc recent VC++ links 'libcmt.lib' instead of 'libc.lib'
  'GL/glut.h' 'glut32.lib' 'glut32.dll' from glut-3.7.6-bin.zip
  'GL/glpng.h' 'glpng.lib' from http://openports.se/graphics/glpng
*/

#include "testLNG2D.h"

using namespace std;

TestLNG2D::TestLNG2D()
{
  // fps->flag_show = false;
  fps->pos = LNGcoord3f(-0.9, -0.5, 0.1);
  fps->col = LNGcolor4f(0.8, 0.4, 0.2, 0.9);
}

TestLNG2D::~TestLNG2D()
{
}

void TestLNG2D::DisplayDraw(void)
{
  LNG2Dframe::DisplayDraw();
}

int main(int ac, char **av)
{
  try{
    TestLNG2D frm;
    string title = "testLNG2D";
#ifdef _DEBUG
    frm.InitFrame(&ac, av, title, LNGsize(240, 320), LNGpoint(40, 40));
#endif
    frm.InitFrame(&ac, av, title);
  }catch(LNGexception &e){
    cout << e.getstr();
  }
  return 0;
}
