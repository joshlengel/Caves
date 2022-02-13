#pragma once

#include<cstdint>
#include<cstddef>
#include<vector>

enum Cell
{
    CELL_EMPTY = 0,
    CELL_WALL
};

/**
 * @brief Represents the grid of cells. Also takes care of processing the results
 * of the cellular automaton.
 * 
 */
class Grid
{
public:
    /**
     * @brief Fills a new Grid object with randomized cells.
     * 
     * @param width the amount of columns
     * @param height the amount of rows
     * @param cell_size_x the width of any cell in normalized device coordinates
     * @param cell_size_y the height of any cell in normalized device coordinates
     * @param fill_amount the likelyhood of any cell being a wall
     */
    Grid(uint32_t width, uint32_t height, float cell_size_x, float cell_size_y, float fill_amount);
    ~Grid();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    float GetCellSizeX() const;
    float GetCellSizeY() const;

    Cell *operator[](size_t i);

    /**
     * @brief Obtain all closed regions of a particular cell type.
     * 
     * @param type the type of regions to find
     * @return list of regions where each region contains a list of x- and y-indices corresponding
     * to a cell in the grid
     */
    std::vector<std::vector<std::pair<uint32_t, uint32_t>>> GetRegions(Cell type) const;

    /**
     * @brief Obtain the boundary of a particular region. This set includes all points
     * in the region for which not all four neighbors are of the same type.
     * 
     * @param region the region for which the boundary should be determined
     * @return the list of x- and y-indices corresponding to the boundary cells in the grid.
     */
    std::vector<std::pair<uint32_t, uint32_t>> GetRegionBoundary(const std::vector<std::pair<uint32_t, uint32_t>> &region) const;

    /**
     * @brief Set all points in the region to the specified type.
     * 
     * @param region the region to fill
     * @param res the cell type with which the region should be filled
     */
    void SetRegion(const std::vector<std::pair<uint32_t, uint32_t>> &region, Cell res);

    /**
     * @brief Apply the following processes:
     * 1. Find all wall regions and remove very small ones.
     * 2. Find all empty regions and remove very small ones.
     * 3. Connect all separate empty regions using the shortest path.
     */
    void Process();

private:
    uint32_t m_width, m_height;
    float m_cell_size_x, m_cell_size_y;
    Cell *m_cells;
};