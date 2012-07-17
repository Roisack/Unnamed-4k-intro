//#include <GL/gl.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <alsa/asoundlib.h>
#include "shader.h"
#include "surface.h"
#include "audio.h"

//#define DEBUG // Prints to console
//#define SHOWFPS
void _start()
{
    srand(1995);
    int w = 800;
    int h = 600;
    SDL_Event e;
    
    SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE);
#ifndef DEBUG
    glewInit();
#else
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error initializing Glew: %s\n", glewGetErrorString(err));
    }
#endif

    float eye_x, eye_y, eye_z;
    float center_x, center_y, center_z = 0;
    float time = 0.0;

    createSurface(1024, 1024);
  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, w/h, 0.1, 1100);
#ifdef SHOWFPS
    int nextFPS = 2000;
#endif
    float demoTime = 0;

    initAudio();
    
    u_int8_t music[4*48000];
    unsigned int note;
    
    for (note = 0; note < 48000; note++)
        music[note] = sqrt(note)*1000;
    for (note = 48000; note < 2*48000; note++)
        music[note] = 2000*sin(note/5);
    
    unsigned int length = 4*48000;

    do {

        playSound(music, length);

        time = 0.001f * SDL_GetTicks();
        float delta = time - demoTime;
        if (delta > 0.5)
            delta = 0;
        demoTime = time;

#ifdef SHOWFPS
        if (SDL_GetTicks() > nextFPS)
        {
            fprintf(stderr, "FPS: %f\n", (1.0f/delta));
            nextFPS += 1000;
        }
#endif

        eye_x = 0 + abs(sin(time*2))*120;
        eye_y = 0 + cos(time*3)*120;
        eye_z = 500 + sin(time*5)*150;

        SDL_PollEvent(&e);

        glClearColor(0.0f, 0.0f, 0.4f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, 0, 1, 0);
        updateSurface(time);
        renderSurface(time);

        unuseShader();
        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);

        SDL_GL_SwapBuffers();
        while (delta < 1.0f / 60.0f)
        {
            delta = (0.001f * SDL_GetTicks()-time);
        }
    } while (e.type != SDL_KEYDOWN);

    closeAudio();

    destroyShader();
    destroySurface();
#ifdef DEBUG
    fprintf(stderr, "SDL_Quit()\n");
#endif
    SDL_Quit();
#ifdef DEBUG
    fprintf(stderr, "Exiting\n");
#endif
    asm ( \
      "movl $1,%eax\n" \
      "xor %ebx,%ebx\n" \
      "int $128\n" \
    );
}

