#include"Utils.h"

#include<stdexcept>

void InitializeBuffers2D(GLuint &vao, GLuint &vbo, GLuint &ibo)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void InitializeShaders(GLuint &program, GLuint &vs, GLuint &fs, const char *v_src, const char *f_src)
{
    program = glCreateProgram();
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &v_src, nullptr);
    glShaderSource(fs, 1, &f_src, nullptr);

    glCompileShader(vs);
    glCompileShader(fs);

    GLint status;

    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
        std::string msg(length, '\0');
        glGetShaderInfoLog(vs, length, nullptr, &msg[0]);
        throw std::runtime_error("Error compiling vertex shader: " + msg);
    }

    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
        std::string msg(length, '\0');
        glGetShaderInfoLog(fs, length, nullptr, &msg[0]);
        throw std::runtime_error("Error compiling fragment shader: " + msg);
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glBindAttribLocation(program, 0, "position");

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string msg(length, '\0');
        glGetProgramInfoLog(program, length, nullptr, &msg[0]);
        throw std::runtime_error("Error linking shader program: " + msg);
    }
}

void GetUniform(GLuint program, const char *name, GLint &loc)
{
    loc = glGetUniformLocation(program, name);
}

void DestroyShaders(GLuint program, GLuint vs, GLuint fs)
{
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}

void DestroyBuffers(GLuint vao, GLuint vbo, GLuint ibo)
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}