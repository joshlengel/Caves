#pragma once

#include<cstdint>
#include<cstddef>
#include<vector>

enum Cell
{
    CELL_EMPTY = 0,
    CELL_WALL
};

class Grid
{
public:
    Grid(uint32_t width, uint32_t height, float cell_size_x, float cell_size_y, float fill_amount);
    ~Grid();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    float GetCellSizeX() const;
    float GetCellSizeY() const;

    Cell *operator[](size_t i);

    std::vector<std::vector<std::pair<uint32_t, uint32_t>>> GetRegions(Cell type) const;
    std::vector<std::pair<uint32_t, uint32_t>> GetRegionBoundary(const std::vector<std::pair<uint32_t, uint32_t>> &region) const;
    void SetRegion(const std::vector<std::pair<uint32_t, uint32_t>> &region, Cell res);

    void Process();

private:
    uint32_t m_width, m_height;
    float m_cell_size_x, m_cell_size_y;
    Cell *m_cells;
};