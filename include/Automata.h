#pragma once

#include"Grid.h"

/**
 * @brief Takes a randomly generated Grid object and smooths the walls
 * by treating each point like a cell in a two-dimensional cellular
 * automaton.
 */
class Automaton
{
public:
    Automaton(Grid &grid);
    ~Automaton();

    /**
     * @brief Steps the simulation forward in time. For any given cell, the next
     * state will be determined as follows:
     * 1. The number of neighboring wall cells are counted (up to eight)
     * 2. If this count is below four, the cell will be empty. If it is above four,
     * it will be a wall. Otherwise, it retains its value.
     */
    void Step();

private:
    Grid &m_grid;
    Cell *m_tmp;
};