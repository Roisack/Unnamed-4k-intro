#include "surface.h"
#include "gekko_math.h"
#include "shader.h"

#define DEBUG // prints to console and error handling
#define FILLSURFACE // SDL surface operations
//#define CLEAN
// TODO: Drop SDL_Surface entirely

struct Surface surface;
struct Surface* render_base_ptr = &surface;

#ifdef FILLSURFACE
/*
void setPixel(SDL_Surface* pSurface, int x, int y, SDL_Color color)
{
    Uint32 col = SDL_MapRGB(pSurface->format, color.r, color.g, color.b);i
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(pPosition, &col, pSurface->format->BytesPerPixel);
}
*/
unsigned int* pixels[3072];
void fillPixels()
{
    fprintf(stderr, "asd2\n");
    int i;
    fprintf(stderr, "asd\n");
    for (i = 0; i < 3072; i++)
    {
        pixels[i] = g_random(0, 255);
    }
    fprintf(stderr, "dos\n");
}
#endif

void createSurface(int w, int h)
{
#ifdef DEBUG
fprintf(stderr, "Creating surface\n");
#endif
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
#ifdef DEBUG
    fprintf(stderr, "SDL_CreateRGBSurface\n");
#endif
    //SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 1024, 1024, 32, 0, 0, 0, 0);
#ifdef DEBUG
    //if (temp == NULL)
    //    fprintf(stderr, "Error at creating SDL_Surface\n");
#endif
    render_base_ptr->format = GL_RGB;
#ifdef FILLSURFACE
    //SDL_LockSurface(temp);
    //SDL_Color col;
    //int i, j;
#ifdef DEBUG
    //fprintf(stderr, "Filling SDL surface\n");
#endif
    /*for (i = 0; i < 1024; i++)
    {
        for (j = 0; j < 1024; j++)
        {
            col.r = g_random(0,255);//linear_interpolation(0, render_base_ptr->width, i);
            col.g = col.r;//linear_interpolation(0, render_base_ptr->height, j);
            col.b = col.r;
            setPixel(temp, i, j, col);
        }
    }
    SDL_UnlockSurface(temp);
    */
#endif
#ifdef DEBUG
    fprintf(stderr, "Creating opengl texture\n");
#endif
    fillPixels();
    fprintf(stderr, "Pixel value is: %d\n", pixels[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, render_base_ptr->format, 1024, 1024, 0, render_base_ptr->format, GL_UNSIGNED_BYTE, pixels);
    fprintf(stderr, "done\n");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    doShader();
    render_base_ptr->shader1 = getShader(0);
    useShader(render_base_ptr->shader1);
}

#ifdef CLEAN
void destroySurface()
{
#ifdef DEBUG
    fprintf(stderr, "Destroying surface\n");
#endif
}
#endif

void renderSurface(float t)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(60.0f,60.0f,0.0f);
    glScalef(0.1f, 0.1f, 0.1f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    //glColor4f(1.0f, 1.0f, 1.0f, render_base_ptr->opacity);

    glDisable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation(GL_ADD);
    
    useShader(render_base_ptr->shader1);
    bind(0, render_base_ptr->id);
    shaderSetInt("base", 0, render_base_ptr->shader1);
    shaderSetFloat("time", t*20+2500, render_base_ptr->shader1); 
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-1024, -1024);
        glTexCoord2f(1, 1); glVertex2f(1024,-1024);
        glTexCoord2f(1, 0); glVertex2f(1024, 1024);
        glTexCoord2f(0, 0); glVertex2f(-1024,1024);
    glEnd();
    glPopMatrix();
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

