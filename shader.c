#include "shader.h"
#include <stdio.h>
struct Shader s1;
struct Shader* s1_ptr = &s1;
struct Shader s2;
struct Shader* s2_ptr = &s2;
struct Shader s3;
struct Shader* s3_ptr = &s3;

//#define DEBUG // Prints to console
//#define VERIFYFILE // Handle file errors
//#define READFROMFILE  // If the shaders are read from an external file
void doShader()
{
#ifndef READFROMFILE
#include "coolShader.h"
#include "perlinShader.h"
#include "interference.h"
#include "phong.h"
    s1_ptr->vs_source = interference_vs;
    s1_ptr->fs_source = interference_fs;
    s2_ptr->vs_source = phong_vs;
    s2_ptr->fs_source = phong_fs;
    s3_ptr->vs_source = perlinNoise_vs;
    s3_ptr->fs_source = perlinNoise_fs;
#else
    s1_ptr->vs_source = loadSource("coolShader.vs");
    s1_ptr->fs_source = loadSource("coolShader.fs");
#endif
#ifdef DEBUG
    fprintf(stderr, "%s\n**%s\n**", s1_ptr->vs_source, s1_ptr->fs_source);
#endif
    compile(s1_ptr);
    compile(s2_ptr);
}

#ifdef READFROMFILE
char* loadSource(const char* filename)
{
    FILE* filePtr;
    char* buffer = NULL;
    int count = 0;
#ifdef VERIFYFILE
    if (filename != NULL)
    {
        filePtr = fopen(filename, "rt");
        if (filePtr != NULL)
        {
#ifdef DEBUG
            fprintf(stderr, "Opened file %s\n", filename);
#endif
            fseek(filePtr, 0, SEEK_END);
            count = ftell(filePtr);
            rewind(filePtr);

            if (count > 0)
            {
                buffer = (char *)malloc(sizeof(char) * (count+1));
                count = fread(buffer,sizeof(char),count,filePtr);
                buffer[count] = '\0';
            }
            fclose(filePtr);
        }
        else
        {
#ifdef DEBUG
            fprintf(stderr, "Could not open file %s\n", filename);
#endif
        }
    }
#else
    filePtr = fopen(filename, "rt");
#ifdef DEBUG
    fprintf(stderr, "Opened file %s\n", filename);
#endif
    fseek(filePtr, 0, SEEK_END);
    count = ftell(filePtr);
    rewind(filePtr);

    if (count > 0)
    {
        buffer = (char *)malloc(sizeof(char) * (count+1));
        count = fread(buffer,sizeof(char),count,filePtr);
        buffer[count] = '\0';
    }
    fclose(filePtr);
#endif
    return buffer;
}
#endif

void compile(struct Shader* toCompile)
{
#ifdef DEBUG
    fprintf(stderr, "Compiling\n");
#endif
    toCompile->vs_handle = glCreateShader(GL_VERTEX_SHADER);
    toCompile->fs_handle = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* vs_src = toCompile->vs_source;
    const GLchar* fs_src = toCompile->fs_source;
    char* infoLog;
    int infoLogLength = 0;  

    glShaderSource(toCompile->vs_handle, 1, &vs_src, NULL);
    glShaderSource(toCompile->fs_handle, 1, &fs_src, NULL);
#ifdef DEBUG
    fprintf(stderr, "---\n\n%s\n\n%s\n\n", toCompile->vs_source, toCompile->fs_source);
#endif

    glCompileShader(toCompile->vs_handle);
#ifdef DEBUG
    printShaderInfoLog(toCompile->vs_handle);
    if (!(glGetError() == GL_NO_ERROR))
    {
        fprintf(stderr, "Error compiling vertex shader %d\n", glGetError());
    }
#endif

    glCompileShader(toCompile->fs_handle);
#ifdef DEBUG
    printShaderInfoLog(toCompile->fs_handle);
    if (!(glGetError() == GL_NO_ERROR))
    {
        fprintf(stderr, "Error compiling fragment shader %d\n", glGetError());
    }
#endif

    toCompile->shader_handle = glCreateProgram();
#ifdef DEBUG
    fprintf(stderr, "shader_handle is %d\n", toCompile->shader_handle);
#endif
    glAttachShader(toCompile->shader_handle, toCompile->vs_handle);
    glAttachShader(toCompile->shader_handle, toCompile->fs_handle);
    glLinkProgram(toCompile->shader_handle);
#ifdef DEBUG
    fprintf(stderr, "shader_handle is %d\n", toCompile->shader_handle);
    printProgramInfoLog(toCompile->shader_handle);
#endif
}

struct Shader* getShader(int id)
{
    if (id == 0)
        return s1_ptr;
    if (id == 1)
        return s2_ptr;
    else
        return NULL;
}


#ifdef DEBUG
void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        fprintf(stderr, "%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

#endif

void destroyShader(struct Shader* s)
{
#ifdef DEBUG
    fprintf(stderr, "Destroying shader\n");
#endif
    glDeleteProgram(s1_ptr->shader_handle);
    glDeleteShader(s->vs_handle);
    glDeleteShader(s->fs_handle);
#ifdef DEBUG
    fprintf(stderr, "Freeing source\n");
#endif
    free(s->vs_source);
#ifdef DEBUG
    fprintf(stderr, "Freeing source\n");
#endif
    free(s->fs_source);
#ifdef DEBUG
    fprintf(stderr, "Shader destroyed\n");
#endif
}

void useShader(struct Shader* s)
{
    glUseProgram(s->shader_handle);
}

void unuseShader()
{
    glUseProgram(0);
}

void shaderSetInt(const char* n, int i, struct Shader* s)
{
    glUniform1i(glGetUniformLocation(s->shader_handle, n), i);
}

void shaderSetFloat(const char* n, float f, struct Shader* s)
{
    glUniform1f(glGetUniformLocation(s->shader_handle, n), f);
}

