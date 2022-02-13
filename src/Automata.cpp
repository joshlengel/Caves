#include"Automata.h"
#include"Grid.h"

#include<cstring>

Automaton::Automaton(Grid &grid):
    m_grid(grid),
    m_tmp(new bool[grid.GetWidth() * grid.GetHeight()])
{}

Automaton::~Automaton()
{
    delete[] m_tmp;
}

void Automaton::Step()
{
    std::memcpy(m_tmp, m_grid[0], m_grid.GetWidth() * m_grid.GetHeight() * sizeof(bool));

    for (uint32_t y = 1; y < m_grid.GetHeight() - 1; ++y)
    for (uint32_t x = 1; x < m_grid.GetWidth() - 1; ++x)
    {
        uint32_t num_active_neighbors = 0;

        if (x == 1 || y == 1 || m_tmp[(y - 1) * m_grid.GetWidth() + (x - 1)])
            ++num_active_neighbors;

        if (x == 1 || m_tmp[y * m_grid.GetWidth() + (x - 1)])
            ++num_active_neighbors;

        if (x == 1 || y == m_grid.GetHeight() - 2 || m_tmp[(y + 1) * m_grid.GetWidth() + (x - 1)])
            ++num_active_neighbors;

        if (y == 1 || m_tmp[(y - 1) * m_grid.GetWidth() + x])
            ++num_active_neighbors;

        if (y == m_grid.GetHeight() - 2 || m_tmp[(y + 1) * m_grid.GetWidth() + x])
            ++num_active_neighbors;

        if (x == m_grid.GetWidth() - 2 || y == 1 || m_tmp[(y - 1) * m_grid.GetWidth() + (x + 1)])
            ++num_active_neighbors;

        if (x == m_grid.GetWidth() - 2 || m_tmp[y * m_grid.GetWidth() + (x + 1)])
            ++num_active_neighbors;

        if (x == m_grid.GetWidth() - 2 || y == m_grid.GetHeight() - 2 || m_tmp[(y + 1) * m_grid.GetWidth() + (x + 1)])
            ++num_active_neighbors;

        m_grid[y][x] = num_active_neighbors > 4? true : (num_active_neighbors < 4? false : m_grid[y][x]);
    }
}