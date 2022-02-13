#pragma once

#include"Grid.h"

class Automaton
{
public:
    Automaton(Grid &grid);
    ~Automaton();

    void Step();

private:
    Grid &m_grid;
    Cell *m_tmp;
};