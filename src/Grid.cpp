#include"Grid.h"
#include"Utils.h"

#include<glad/glad.h>

#include<cstring>
#include<random>

struct GridData
{
    GLuint vao;
    GLuint vbo, ibo;
    GLuint program, vs, fs;

    GLint loc_offset, loc_size;
};

static const float VERTICES[] =
{
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
};

static const unsigned int INDICES[] =
{
    0, 1, 3,
    0, 3, 2
};

static const char *const VERTEX_SRC = R"(
    #version 130

    in vec2 position;

    uniform vec2 offset;
    uniform vec2 size;

    void main() {
        gl_Position = vec4(position * size * 0.5 + offset, 0.0, 1.0);
    }
)";

static const char *const FRAGMENT_SRC = R"(
    #version 130

    out vec4 color;

    void main() {
        color = vec4(1.0);
    }
)";

Grid::Grid(uint32_t width, uint32_t height, float cell_size_x, float cell_size_y, float fill_amount):
    m_width(width), m_height(height),
    m_cell_size_x(cell_size_x), m_cell_size_y(cell_size_y),
    m_cells(new bool[width * height]),
    m_data(new GridData)
{
    // Randomize cells
    uint32_t index = 0;
    for (uint32_t y = 0; y < m_height; ++y)
    for (uint32_t x = 0; x < m_width; ++x)
    {
        m_cells[index++] = x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1 || static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < fill_amount;
    }

    // Setup OpenGL stuff
    InitializeBuffers2D(m_data->vao, m_data->vbo, m_data->ibo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    InitializeShaders(m_data->program, m_data->vs, m_data->fs, VERTEX_SRC, FRAGMENT_SRC);
    GetUniform(m_data->program, "offset", m_data->loc_offset);
    GetUniform(m_data->program, "size", m_data->loc_size);
}

Grid::~Grid()
{
    DestroyBuffers(m_data->vao, m_data->vbo, m_data->ibo);
    DestroyShaders(m_data->program, m_data->vs, m_data->fs);

    delete[] m_cells;
    delete m_data;
}

uint32_t Grid::GetWidth() const { return m_width; }
uint32_t Grid::GetHeight() const { return m_height; }
float Grid::GetCellSizeX() const { return m_cell_size_x; }
float Grid::GetCellSizeY() const { return m_cell_size_y; }

bool *Grid::operator[](size_t i) { return m_cells + i * m_width; }

void Grid::Render()
{
    glUseProgram(m_data->program);
    glUniform2f(m_data->loc_size, m_cell_size_x, m_cell_size_y);

    glBindVertexArray(m_data->vao);

    uint32_t index = 0;
    for (uint32_t y = 0; y < m_height; ++y)
    for (uint32_t x = 0; x < m_width; ++x)
    {
        if (!m_cells[index++])
            continue;
        
        float off_x = (static_cast<float>(x) + 0.5f) * m_cell_size_x - 1.0f;
        float off_y = (static_cast<float>(y) + 0.5f) * m_cell_size_y - 1.0f;
        glUniform2f(m_data->loc_offset, off_x, off_y);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}