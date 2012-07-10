#include "shader.h"
#include <stdio.h>
struct Shader s1;
struct Shader* s1_ptr = &s1;

//#define DEBUG // Prints to console
//#define VERIFYFILE // Handle file errors
//#define READFROMFILE  // If the shaders are read from an external file
void doShader()
{
#ifndef READFROMFILE
#include "coolShader.h"
    s1_ptr->vs_source = coolShader_vs;
    s1_ptr->fs_source = coolShader_fs;
#else
    s1_ptr->vs_source = loadSource("coolShader.vs");
    s1_ptr->fs_source = loadSource("coolShader.fs");
#endif
#ifdef DEBUG
    fprintf(stderr, "%s\n**%s\n**", s1_ptr->vs_source, s1_ptr->fs_source);
#endif
    compile();
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

void compile()
{
#ifdef DEBUG
    fprintf(stderr, "Compiling\n");
#endif
    s1_ptr->vs_handle = glCreateShader(GL_VERTEX_SHADER);
    s1_ptr->fs_handle = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* vs_src = s1_ptr->vs_source;
    const GLchar* fs_src = s1_ptr->fs_source;
    char* infoLog;
    int infoLogLength = 0;  

    glShaderSource(s1_ptr->vs_handle, 1, &vs_src, NULL);
    glShaderSource(s1_ptr->fs_handle, 1, &fs_src, NULL);
#ifdef DEBUG
    fprintf(stderr, "---\n\n%s\n\n%s\n\n", s1_ptr->vs_source, s1_ptr->fs_source);
#endif

    glCompileShader(s1_ptr->vs_handle);
#ifdef DEBUG
    printShaderInfoLog(s1_ptr->vs_handle);
    if (!(glGetError() == GL_NO_ERROR))
    {
        fprintf(stderr, "Error compiling vertex shader %d\n", glGetError());
    }
#endif

    glCompileShader(s1_ptr->fs_handle);
#ifdef DEBUG
    printShaderInfoLog(s1_ptr->fs_handle);
    if (!(glGetError() == GL_NO_ERROR))
    {
        fprintf(stderr, "Error compiling fragment shader %d\n", glGetError());
    }
#endif

    s1_ptr->shader_handle = glCreateProgram();
#ifdef DEBUG
    fprintf(stderr, "shader_handle is %d\n", s1_ptr->shader_handle);
#endif
    glAttachShader(s1_ptr->shader_handle, s1_ptr->vs_handle);
    glAttachShader(s1_ptr->shader_handle, s1_ptr->fs_handle);
    glLinkProgram(s1_ptr->shader_handle);
#ifdef DEBUG
    fprintf(stderr, "shader_handle is %d\n", s1_ptr->shader_handle);
    printProgramInfoLog(s1_ptr->shader_handle);
#endif
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

void destroyShader()
{
#ifdef DEBUG
    fprintf(stderr, "Destroying shader\n");
#endif
    glDeleteProgram(s1_ptr->shader_handle);
    glDeleteShader(s1_ptr->vs_handle);
    glDeleteShader(s1_ptr->fs_handle);
#ifdef DEBUG
    fprintf(stderr, "Freeing source\n");
#endif
    free(s1_ptr->vs_source);
#ifdef DEBUG
    fprintf(stderr, "Freeing source\n");
#endif
    free(s1_ptr->fs_source);
#ifdef DEBUG
    fprintf(stderr, "Shader destroyed\n");
#endif
}

void useShader()
{
    glUseProgram(s1_ptr->shader_handle);
}

void unuseShader()
{
    glUseProgram(0);
}

void shaderSetInt(const char* n, int i)
{
    glUniform1i(glGetUniformLocation(s1_ptr->shader_handle, n), i);
}

void shaderSetFloat(const char* n, float f)
{
    glUniform1f(glGetUniformLocation(s1_ptr->shader_handle, n), f);
}

