#include"Automata.h"

#include<cstring>

Automaton::Automaton(Grid &grid):
    m_grid(grid),
    m_tmp(new Cell[grid.GetWidth() * grid.GetHeight()])
{}

Automaton::~Automaton()
{
    delete[] m_tmp;
}

void Automaton::Step()
{
    // Save current state
    std::memcpy(m_tmp, m_grid[0], m_grid.GetWidth() * m_grid.GetHeight() * sizeof(Cell));

    for (uint32_t y = 1; y < m_grid.GetHeight() - 1; ++y)
    for (uint32_t x = 1; x < m_grid.GetWidth() - 1; ++x)
    {
        // Count the amount of neighboring walls (including edges)
        uint32_t num_active_neighbors = 0;

        // bottom left
        if (x == 1 || y == 1 || m_tmp[(y - 1) * m_grid.GetWidth() + (x - 1)] == CELL_WALL)
            ++num_active_neighbors;

        // bottom center
        if (x == 1 || m_tmp[y * m_grid.GetWidth() + (x - 1)] == CELL_WALL)
            ++num_active_neighbors;

        // bottom right
        if (x == 1 || y == m_grid.GetHeight() - 2 || m_tmp[(y + 1) * m_grid.GetWidth() + (x - 1)] == CELL_WALL)
            ++num_active_neighbors;

        // center left
        if (y == 1 || m_tmp[(y - 1) * m_grid.GetWidth() + x] == CELL_WALL)
            ++num_active_neighbors;

        // center right
        if (y == m_grid.GetHeight() - 2 || m_tmp[(y + 1) * m_grid.GetWidth() + x] == CELL_WALL)
            ++num_active_neighbors;

        // top left
        if (x == m_grid.GetWidth() - 2 || y == 1 || m_tmp[(y - 1) * m_grid.GetWidth() + (x + 1)] == CELL_WALL)
            ++num_active_neighbors;

        // top center
        if (x == m_grid.GetWidth() - 2 || m_tmp[y * m_grid.GetWidth() + (x + 1)] == CELL_WALL)
            ++num_active_neighbors;

        // top right
        if (x == m_grid.GetWidth() - 2 || y == m_grid.GetHeight() - 2 || m_tmp[(y + 1) * m_grid.GetWidth() + (x + 1)] == CELL_WALL)
            ++num_active_neighbors;

        // Apply rule
        m_grid[y][x] = num_active_neighbors > 4? CELL_WALL : (num_active_neighbors < 4? CELL_EMPTY : m_grid[y][x]);
    }
}