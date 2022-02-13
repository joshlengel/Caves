#pragma once

#include<glad/glad.h>

void InitializeBuffers2D(GLuint &vao, GLuint &vbo, GLuint &ibo);
void InitializeShaders(GLuint &program, GLuint &vs, GLuint &fs, const char *v_src, const char *f_src);
void GetUniform(GLuint program, const char *name, GLint &loc);
void DestroyShaders(GLuint program, GLuint vs, GLuint fs);
void DestroyBuffers(GLuint vao, GLuint vbo, GLuint ibo);