#include <GL/gl.h>

struct Shader
{
    char* vs_source;
    char* fs_source;
    GLuint shader_handle;
    GLuint vs_handle;
    GLuint fs_handle;
};

void doShader();
char* loadSource();
void compile();

void shaderSetInt(const char* n, int i);
void shaderSetFloat(const char* n, float f);
void useShader();
void unuseShader();
void destroyShader();

