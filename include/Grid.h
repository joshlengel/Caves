#pragma once

#include<cstdint>
#include<cstddef>

class Grid
{
public:
    Grid(uint32_t width, uint32_t height, float cell_size_x, float cell_size_y, float fill_amount);
    ~Grid();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    float GetCellSizeX() const;
    float GetCellSizeY() const;

    bool *operator[](size_t i);

private:
    uint32_t m_width, m_height;
    float m_cell_size_x, m_cell_size_y;
    bool *m_cells;
};