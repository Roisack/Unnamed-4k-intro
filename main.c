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
//    srand(1995);
    SDL_Event e;
    
    SDL_SetVideoMode(800, 600, 32, SDL_OPENGL | SDL_RESIZABLE);
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
    struct Shader* shader1 = getShader(0);
    struct Shader* shader2 = getShader(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, 800/600, 0.1, 1100);
#ifdef SHOWFPS
    int nextFPS = 2000;
#endif
    float demoTime = 0;

    initAudio();
    
    int song[1024];       // Contains the note as a frequency
    int noteLength[1024]; // Contains the length of this note (45000 == 1 sec)
    int instrument[1024]; // Contains the number of modulation to be used
    int i;

    /*
    for (i = 0; i < 32; i++)
    {
        if (i % 3 == 0)
            song[i] = 440;
        else if (i % 3 == 1)
            song[i] = 369;
        else if (i % 3 == 2)
            song[i] = 523;
        noteLength[i] = 2000;
    }
    */
    
    for (i = 0; i < 1024; i++)
    {
        song[i] = 440;
        if ( i % 16 == 0 || i % 16 == 1 && i > 2)
        {
            instrument[i] = 0;
            noteLength[i] = 8000;
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
/*
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // Light and material settings
    GLfloat mat_specular[] = { 1.0, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 15.0 };
    GLfloat mat_diffuse[] = { 0.5, 0.5, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_position[] = { 30.0, 50.0, 30.0, 1.0 };
    GLfloat light_specular[] = { 0.8, 0.7, 0.5, 1.0 };
    GLfloat light_ambient[] = { 0.4, 0.4, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_spot_direction[] = { -1.0, -1.0, 0.0 };
*/
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glShadeModel(GL_SMOOTH);

    do {

        streamAudio(song, noteLength, totalNotes, instrument);

        time = 0.001f * SDL_GetTicks();
        float delta = time - demoTime;
//        if (delta > 0.5)
//            delta = 0;
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
        //light_position[0] = sin(time) * 1000;
        //light_position[1] = sin(time) * 1000;

        SDL_PollEvent(&e);

        //glClearColor(0.0f, 0.0f, 0.4f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, 0, 1, 0);
        renderSurface(time);
        unuseShader(shader1);

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glPushMatrix();
        //glTranslatef(0.0f, 0.0f, -5.0f);
        
        /*glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.5);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        */
        //useShader(shader2);
        // Render a floor
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(-1000.0f, -5.0f, 1000.0f);
            glVertex3f(1000.0f, -5.0f, 1000.0f);
            glVertex3f(-1000, -5, -1000.0f);
            glVertex3f(1000, -5, -1000.0f);
        glEnd();

        //unuseShader();
        glPopMatrix();
        SDL_GL_SwapBuffers();
        while (delta < 1.0f / 60.0f)
        {
            delta = (0.001f * SDL_GetTicks()-time);
        }
    } while (e.type != SDL_KEYDOWN);

    closeAudio();

    //destroyShader(shader1);
    //destroyShader(shader2);
    //destroySurface();
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

