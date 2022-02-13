#include"Grid.h"

#include<cstring>
#include<random>

Grid::Grid(uint32_t width, uint32_t height, float cell_size_x, float cell_size_y, float fill_amount):
    m_width(width), m_height(height),
    m_cell_size_x(cell_size_x), m_cell_size_y(cell_size_y),
    m_cells(new bool[width * height])
{
    // Randomize cells
    uint32_t index = 0;
    for (uint32_t y = 0; y < m_height; ++y)
    for (uint32_t x = 0; x < m_width; ++x)
    {
        m_cells[index++] = x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1 || static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < fill_amount;
    }
}

Grid::~Grid()
{
    delete[] m_cells;
}

uint32_t Grid::GetWidth() const { return m_width; }
uint32_t Grid::GetHeight() const { return m_height; }
float Grid::GetCellSizeX() const { return m_cell_size_x; }
float Grid::GetCellSizeY() const { return m_cell_size_y; }

bool *Grid::operator[](size_t i) { return m_cells + i * m_width; }