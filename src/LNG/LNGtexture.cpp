/*
  LNGtexture.cpp
*/

#include <GL/LNG/LNGtexture.h>

using namespace std;

string const LNGtexture::default_resource_dir("resource");
GLuint const LNGtexture::default_depth = 4;
LNGsize const LNGtexture::default_size(256, 256);

LNGtexture::LNGtexture(bool ac, bool cp, bool cd, bool kb,
  GLuint adepth, LNGsize asize) : loading(true), blocking(false),
  use_alphacallback(ac), use_custompixel(cp), use_customdata(cd),
  keep_buffer(kb), buffer(0), depth(adepth), size(asize), id(0)
{
#if defined( __TRACE_CONSTRUCTION__ ) || defined( _DEBUG )
  cout << "LNGtexture::LNGtexture" << endl;
#endif
}

LNGtexture::~LNGtexture()
{
#if defined( __TRACE_DESTRUCTION__ ) || defined( _DEBUG )
  cout << "LNGtexture::~LNGtexture" << endl;
#endif
  Finalize();
}

void LNGtexture::Finalize(void)
{
#if defined( __TRACE_FINALIZATION__ ) || defined( _DEBUG )
  cout << "LNGtexture::Finalize" << endl;
#endif
  if(!loading && buffer){
#if defined( __TRACE_FINALIZATION__ ) || defined( _DEBUG )
    cout << "finalize texture id: " << id << endl;
#endif
  }
  if(buffer){ delete[] buffer; buffer = 0; }
}

GLuint LNGtexture::Load(std::string &filename, bool custom,
  std::string const &resource_dir)
{
  if(blocking) return 0;
  blocking = true;
#if defined( __TRACE_CREATION__ ) || defined( _DEBUG )
  cout << "LNGtexture::Load" << endl;
#endif
  string &filepath = LNGut::path_join(2, &resource_dir, &filename);
#if defined( __TRACE_CREATION__ ) || defined( _DEBUG )
  cout << "loading texture: " << filepath;
  cout.flush();
#endif
  if(!custom){
    if(keep_buffer)
      throw LNGexception(string("auto loading with keep_buffer: ") + filepath);
    pngInfo pi;
    if(use_alphacallback) pngSetAlphaCallback(AlphaCallback);
    id = pngBind(filepath.c_str(), PNG_NOMIPMAP,
      use_alphacallback ? PNG_CALLBACK : PNG_ALPHA,
      &pi, GL_CLAMP, GL_NEAREST, GL_NEAREST);
#if defined( __TRACE_CREATION__ ) || defined( _DEBUG )
    cout << " id: " << id << endl;
#endif
    if(!id) throw LNGexception(string("cannot load texture: ") + filepath);
    loading = false;
    blocking = false;
    return id;
  }
  glGenTextures(1, &id);
#if defined( __TRACE_CREATION__ ) || defined( _DEBUG )
  cout << " id: " << id << endl;
#endif
  if(!id) throw LNGexception(string("cannot generate texture: ") + filepath);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  pngRawInfo pri;
  if(!pngLoadRaw(filepath.c_str(), &pri))
    throw LNGexception(string("cannot load texture file: ") + filepath);
#ifdef _DEBUG
  cout << "File: " << filepath << endl;
  cout << " Width: " << pri.Width << endl;
  cout << " Height: " << pri.Height << endl;
  cout << " Depth: " << pri.Depth << endl;
  cout << " Alpha: " << pri.Alpha << endl;
  cout << " Components: " << pri.Components << endl;
  cout << " pData: " << (long)pri.Data << endl;
  cout << " pPalette: " << (long)pri.Palette << endl;
#endif
  if(pri.Data){
    if(pri.Components == 1 && !pri.Palette)
      throw LNGexception(filepath + " texture palette error NULL");
    if(pri.Components != depth && pri.Components != 1){
      ostringstream oss;
      oss << " texture depth error " << pri.Components;
      throw LNGexception(filepath + oss.str());
    }
#ifdef _DEBUG
    if(pri.Width != size.w){
      ostringstream oss;
      oss << " texture width error " << pri.Width << ", expected " << size.w;
      throw LNGexception(filepath + oss.str());
    }
    if(pri.Height != size.h){
      ostringstream oss;
      oss << " texture height error " << pri.Height << ", expected " << size.h;
      throw LNGexception(filepath + oss.str());
    }
#endif
    // depth = pri.Components;
    size.w = pri.Width;
    size.h = pri.Height;
    GLubyte *buf = 0;
    if(!buf) buf = new GLubyte[depth * size.w * size.h];
    if(!buf) throw LNGexception("cannot allocate buf for LNGtexture");
#if 0
    if(pri.Components == 1){
      ostringstream oss;
      oss << "index color map of texture: " << filepath;
      for(int j = 0; j < 256; j++){
        if(!(j % 8))
          oss << endl << setw(4) << setfill(' ') << dec << right << j << ": ";
        if(j % 8) oss << " ";
        for(int i = 0; i < 3; i++){
          oss << setw(2) << setfill('0') << hex << right;
          oss << (int)pri.Palette[j * 3 + i];
        }
      }
      throw LNGexception(oss.str());
    }
#else
    if(pri.Components == 1){ // There is a bug in glpng.lib (index color 125).
      GLubyte p[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xFF, 0xFF, 0x7F};
      for(int j = 0; j < sizeof(p) / sizeof(GLubyte); j++)
        pri.Palette[j] = p[j];
    }
#endif
    for(int y = 0; y < pri.Height; y++){
      for(int x = 0; x < pri.Width; x++){
        int s = depth - 1;
        int r = (pri.Height - 1 - y) * pri.Width + x; // reverse top and bottom
        int q = (y * pri.Width + x) * depth;
        int p = r * depth;
        if(pri.Components == 1){
          int o = pri.Data[r] * s;
          for(int j = 0; j < s; j++) buf[q + j] = pri.Palette[o + j];
        }else{
          for(int j = 0; j < s; j++) buf[q + j] = pri.Data[p + j];
        }
        if(use_alphacallback)
          buf[q + 3] = CustomAlphaCallback(buf[q + 0], buf[q + 1], buf[q + 2]);
        else buf[q + 3] = (pri.Components == 1) ? 255 : pri.Data[p + 3];
        if(use_custompixel) CustomPixel(&buf[q]);
      }
    }
    if(use_customdata) CustomData(buf);
    if(keep_buffer){
      if(!buffer) buffer = new GLubyte[depth * size.w * size.h];
      if(!buffer) throw LNGexception("cannot allocate buffer for LNGtexture");
      for(int y = 0; y < pri.Height; y++){
        for(int x = 0; x < pri.Width; x++){
          int q = (y * pri.Width + x) * depth;
          for(int j = 0; j < depth; j++) buffer[q + j] = buf[q + j];
        }
      }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, pri.Width, pri.Height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, buf);
    if(buf){ delete[] buf; buf = 0; }
    free(pri.Data);
  }
  if(pri.Palette) free(pri.Palette);
  loading = false;
  blocking = false;
  return id;
}

LNGloader::LNGloader(int size) : loading(true)
{
  if(!textures) textures = new deque<LNGtexture *>(size);
  if(!textures) throw LNGexception("cannot create std::deque<LNGtexture *>");
}

LNGloader::~LNGloader()
{
  if(textures){
    while(!textures->empty()){
      // may use (*textures)[0]; or textures->at(0);
      LNGtexture *texture = textures->front();
      textures->pop_front();
      texture->Finalize();
      delete texture;
    }
    delete textures;
  }
}

void LNGloader::InitLoad(void)
{
  for(int i = 0; i < 3; i++) textures->push_back(new LNGtexture());
}

void LNGloader::LoadNext(void)
{
  bool exist = false;
  deque<LNGtexture *>::iterator it;
  for(it = textures->begin(); it != textures->end(); ++it){
    if(!(*it)->loading) continue;
    if((*it)->blocking) continue;
#if 0
    GLuint id = (*it)->Load(string("72dpi.png"), true);
    GLuint id = (*it)->Load(string("72dpi_ascii_reigasou_16x16.png"), true);
#else
    GLuint id = (*it)->Load(string("f0.png"), true);
#endif
    exist = true;
    break; // load only 1 texture
  }
  if(!exist) loading = false;
}
