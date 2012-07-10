#include "surface.h"
#include "gekko_math.h"
#include "shader.h"

//#define DEBUG // prints to console and error handling
//#define FILLSURFACE // SDL surface operations

struct Surface surface;
struct Surface* render_base_ptr = &surface;

#ifdef FILLSURFACE
void setPixel(SDL_Surface* pSurface, int x, int y, SDL_Color color)
{
    Uint32 col = SDL_MapRGB(pSurface->format, color.r, color.g, color.b);
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(pPosition, &col, pSurface->format->BytesPerPixel);
}
#endif

void createSurface(int w, int h)
{
#ifdef DEBUG
    fprintf(stderr, "Creating surface\n");
#endif
    render_base_ptr->height = h;
    render_base_ptr->width = w;
    render_base_ptr->id = 0;
#ifdef DEBUG
    fprintf(stderr, "glGenTextures\n");
#endif
    glGenTextures(1, &render_base_ptr->id);
#ifdef DEBUG
    if (render_base_ptr->id == 0)
    {
        fprintf(stderr, "-- Error at creating glTexture --\n");
    }
#endif
    render_base_ptr->opacity = 1.0f;
#ifdef DEBUG
    fprintf(stderr, "glBindTexture\n");
#endif
    glBindTexture(GL_TEXTURE_2D, render_base_ptr->id);
    GLuint target = GL_TEXTURE_2D;
    fprintf(stderr, "SDL_CreateRGBSurface\n");
    SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, render_base_ptr->width, render_base_ptr->height, 32, 0, 0, 0, 0);
#ifdef DEBUG
    if (temp == NULL)
        fprintf(stderr, "Error at creating SDL_Surface\n");
#endif
    render_base_ptr->format = GL_RGB;
#ifdef FILLSURFACE
    SDL_LockSurface(temp);
    SDL_Color col;
    int i, j;
#ifdef DEBUG
    fprintf(stderr, "Filling SDL surface\n");
#endif
    for (i = 0; i < render_base_ptr->width; i++)
    {
        for (j = 0; j < render_base_ptr->height; j++)
        {
            col.r = 0.5f;//linear_interpolation(0, render_base_ptr->width, i);
            col.g = 0.5f;//linear_interpolation(0, render_base_ptr->height, j);
            col.b = 0.5f;
            setPixel(temp, i, j, col);
        }
    }
    SDL_UnlockSurface(temp);
#endif
#ifdef DEBUG
    fprintf(stderr, "Creating opengl texture\n");
#endif
    glTexImage2D(target, 0, render_base_ptr->format, render_base_ptr->width, render_base_ptr->height, 0, render_base_ptr->format, GL_UNSIGNED_BYTE, temp->pixels);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    doShader();
    useShader();
}

void destroySurface()
{
#ifdef DEBUG
    fprintf(stderr, "Destroying surface\n");
#endif
}

void renderSurface()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0.1f, 0.1f, 0.1f);
    glScalef(5.0f,5.0f,5.0f);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    //glColor4f(1.0f, 1.0f, 1.0f, render_base_ptr->opacity);

    glDisable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation(GL_ADD);
    
    useShader();
    bind(0, render_base_ptr->id);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-512, -512);
        glTexCoord2f(1, 1); glVertex2f(512,-512);
        glTexCoord2f(1, 0); glVertex2f(512, 512);
        glTexCoord2f(0, 0); glVertex2f(-512,512);
    glEnd();
    glPopMatrix();
}

void updateSurface(float t)
{
    shaderSetFloat("time", t);
}

void bind(int unit, int id)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void unbind()
{
    int i;
    for (i = 0; i < 8; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

