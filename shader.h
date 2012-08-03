//#include <GL/gl.h>
#include <GL/glew.h>
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
void compile(struct Shader* s);
struct Shader* getShader(int id);

void shaderSetInt(const char* n, int i, struct Shader* s);
void shaderSetFloat(const char* n, float f, struct Shader* s);
void useShader(struct Shader* s);
void unuseShader();
void destroyShader(struct Shader* s);

