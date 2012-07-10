#include <GL/gl.h>
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
};

void createSurface();
void destroySurface();

void setPixel(SDL_Surface* p, int i, int j, SDL_Color c);

void renderSurface();
void updateSurface(float t);

void bind();
void unbind();

