#pragma once

class Grid;

class Automaton
{
public:
    Automaton(Grid &grid);
    ~Automaton();

    void Step();

private:
    Grid &m_grid;
    bool *m_tmp;
};