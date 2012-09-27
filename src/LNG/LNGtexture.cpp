/*
  LNGtexture.cpp
*/

#include <GL/LNG/LNGtexture.h>

using namespace std;

string const LNGtexture::default_resource_dir("resource");
GLuint const LNGtexture::default_depth = 4;
LNGsize const LNGtexture::default_size(256, 256);

LNGtexture::LNGtexture(GLuint adepth, LNGsize asize) : flag_loading(true),
  buffer(0), depth(adepth), size(asize), id(0)
{
#if defined( __TRACE_CONSTRUCTION__ ) || defined( _DEBUG )
  cout << "LNGtexture::LNGtexture" << endl;
#endif
  if(!buffer) buffer = new GLubyte[depth * asize.w * asize.h];
  if(!buffer) throw LNGexception("cannot allocate buffer for LNGtexture");
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
  if(!flag_loading && buffer){
#if defined( __TRACE_FINALIZATION__ ) || defined( _DEBUG )
    cout << "finalize texture id: " << id << endl;
#endif
  }
  if(buffer){ delete[] buffer; buffer = 0; }
}

GLuint LNGtexture::Load(std::string &filename,
  std::string const &resource_dir)
{
#if defined( __TRACE_CREATION__ ) || defined( _DEBUG )
  cout << "LNGtexture::Load" << endl;
#endif
  string &filepath = LNGut::path_join(2, &resource_dir, &filename);
#if defined( __TRACE_CREATION__ ) || defined( _DEBUG )
  cout << "loading texture: " << filepath;
  cout.flush();
#endif
  id = 1;
#if defined( __TRACE_CREATION__ ) || defined( _DEBUG )
  cout << " id: " << id << endl;
#endif
  if(!id) throw LNGexception(string("cannot load texture: ") + filepath);
  flag_loading = false;
  return id;
}

GLubyte *LNGtexture::CustomData(LNGsize size, GLubyte *buf)
{
  return buf;
}

LNGloader::LNGloader(int size) : flag_loading(true)
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
    if(!(*it)->flag_loading) continue;
    GLuint id = (*it)->Load(string("72dpi.png"));
    exist = true;
    break; // load only 1 texture
  }
  if(!exist) flag_loading = false;
}
