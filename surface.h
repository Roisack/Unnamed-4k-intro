#include <GL/glew.h>
#include <SDL/SDL.h>

//#define CLEAN

struct Surface
{
    GLuint id;  // The OpenGL handle
    GLenum format;  // RGB, RGBA, LUMINANCE
    float opacity;  // How transparent this is
    struct Shader* shader1;
};

void createSurface();
#ifdef CLEAN
void destroySurface();
#endif

//void setPixel(SDL_Surface* p, int i, int j, SDL_Color c);

void renderSurface(float t);

void bind();
void unbind();

