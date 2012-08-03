#include <GL/glew.h>
#include <SDL/SDL.h>

struct Surface
{
    // Coordinates of a 3D plane corner vertices
    GLfloat x_start;
    GLfloat y_start;
    GLfloat x_end;
    GLfloat y_end;

    int width, height;

    GLuint id;  // The OpenGL handle
    GLenum format;  // RGB, RGBA, LUMINANCE
    float opacity;  // How transparent this is
    struct Shader* shader1;
};

void createSurface();
void destroySurface();

void setPixel(SDL_Surface* p, int i, int j, SDL_Color c);

void renderSurface(float t);

void bind();
void unbind();

