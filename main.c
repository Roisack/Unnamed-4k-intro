#include <SDL/SDL.h>
#include <GL/glew.h>
#include <alsa/asoundlib.h>
#include "shader.h"
#include "surface.h"
#include "audio.h"

//#define DEBUG // Prints to console
//#define SHOWFPS

// OpenGL context problem: If using void _start as entrypoint
// glMaterialfv will cause a segfault. About 50 bytes are lost
// by using void _start
//#define USEHACKENTRY

#ifdef USEHACKENTRY
void _start()
#else
int main()
#endif
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
    
    int song[1024];       // Contains the note as a frequency
    int noteLength[1024]; // Contains the length of this note (45000 == 1 sec)
    int instrument[1024]; // Contains the number of modulation to be used
    int i;

    int count = 1;
    for (i = 0; i < 1024; i++)
    {
        song[i] = 440;

        if ( i % 16 == 0 || i % 16 == 1 && i > 2)
        {
            instrument[i] = 2;
            noteLength[i] = 4000;
        }
        else if (i % 16 == 2)
        {
            instrument[i] = 4;
            noteLength[i] = 5000;
        }
        else if (i % 8 == 0)
        {
            instrument[i] = 1;
            noteLength[i] = 5000;
        } else if (i % 8 == 1 || i % 8 == 2 || i % 8 == 3)
        {
            instrument[i] = 3;
            noteLength[i] = 5000;
        } else if (i % 8 == 4)
        {
            instrument[i] = 2;
            noteLength[i] = 5000;
        } else
        {
            instrument[i] = 3;
            noteLength[i] = 5000;
        }
    }

    int totalNotes = 1024;

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // Light settings
    GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 6.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    do {

        //streamAudio(song, noteLength, totalNotes, instrument);

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

        eye_x = 100;// + abs(sin(time*2))*120;
        eye_y = 100;// + cos(time*3)*120;
        eye_z = 400;// + sin(time*5)*150;

        SDL_PollEvent(&e);

        glClearColor(0.0f, 0.0f, 0.4f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, 0, 1, 0);
        updateSurface(time);
        renderSurface(time);
        unuseShader();

       glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
       glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        // Render a floor
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(-100.0f, 100.0f, -5.0f);
            glVertex3f(100.0f, 100.0f, -5.0f);
            glVertex3f(-100, -100, -5.0f);
            glVertex3f(100, -100, -5.0f);
        glEnd();

        glPushMatrix();

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
#ifdef USEHACKENTRY
    asm ( \
      "movl $1,%eax\n" \
      "xor %ebx,%ebx\n" \
      "int $128\n" \
    );
#else
    return 0;
#endif
}

