/*
  LNGchar_texture.cpp
*/

#include <LNG/LNGchar_texture.h>

using namespace std;

LNGchar_texture::LNGchar_texture(string &afilename,
  bool kb, bool ac, bool cp, bool cd, GLuint abpp, LNGsize asize,
  string const &aresource_dir) : LNGtexture(afilename,
  kb, ac, cp, cd, abpp, asize, aresource_dir)
{
}

LNGchar_texture::~LNGchar_texture()
{
}
