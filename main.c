#include <GL/gl.h>
#include <SDL/SDL.h>
#include "shader.h"
#include "surface.h"

//#define DEBUG // Prints to console

void _start()
{

    int w = 800;
    int h = 600;

    SDL_Event e;

    SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE);

    float eye_x, eye_y, eye_z;
    float center_x, center_y, center_z;
    center_z = 0;
    float time = 0.0;

    createSurface(1024, 1024);
  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    gluPerspective(65, w/h, 0.1, 1100);

    do {
        time += 0.1;
        //eye_x = sin(time / 3)*150;
        //eye_y = sin(time / 5)*150;
        //eye_z = 100;
        eye_x = 50;
        eye_y = 50;
        eye_z = 60;
        center_x = 0;
        center_y = 0;
        center_z = 0;

        SDL_PollEvent(&e);

        glClearColor(0.0f, 0.0f, 0.4f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, 0, 1, 0);
        updateSurface(time);
        renderSurface();

  //      unuseShader();
        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
            glVertex3f(0.0f,0.0f,0.0f);
            glVertex3f(100.0f, 1.0f, 1.0f);
            glVertex3f(-200.0f, -200.5f, -100.5f);
        glEnd();
        glPopMatrix();
        SDL_GL_SwapBuffers();
    } while (e.type != SDL_KEYDOWN);

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

