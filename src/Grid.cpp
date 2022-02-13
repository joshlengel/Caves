#include"Grid.h"

#include<cstring>
#include<random>
#include<queue>
#include<algorithm>

Grid::Grid(uint32_t width, uint32_t height, float cell_size_x, float cell_size_y, float fill_amount):
    m_width(width), m_height(height),
    m_cell_size_x(cell_size_x), m_cell_size_y(cell_size_y),
    m_cells(new Cell[width * height])
{
    // Randomize cells
    uint32_t index = 0;
    for (uint32_t y = 0; y < m_height; ++y)
    for (uint32_t x = 0; x < m_width; ++x)
    {
        m_cells[index++] = x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1 || static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < fill_amount? CELL_WALL : CELL_EMPTY;
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

Cell *Grid::operator[](size_t i) { return m_cells + i * m_width; }

std::vector<std::vector<std::pair<uint32_t, uint32_t>>> Grid::GetRegions(Cell type) const
{
    std::vector<std::vector<std::pair<uint32_t, uint32_t>>> regions;

    bool *checked = new bool[m_width * m_height];
    std::memset(checked, 0, m_width * m_height * sizeof(bool));

    auto GetRegion = [&](uint32_t x, uint32_t y)
    {
        std::vector<std::pair<uint32_t, uint32_t>> tiles;
        std::queue<std::pair<uint32_t, uint32_t>> to_process;
        to_process.push({ x, y });
        checked[y * m_width + x] = true;

        while (!to_process.empty())
        {
            auto[nx, ny] = to_process.front();
            tiles.push_back({ nx, ny });
            to_process.pop();

            // Check neighbors
            uint32_t left = ny * m_width + (nx - 1);
            uint32_t right = ny * m_width + (nx + 1);
            uint32_t bottom = (ny - 1) * m_width + nx;
            uint32_t top = (ny + 1) * m_width + nx;

            if (nx > 0 && !checked[left] && m_cells[left] == type)
            {
                to_process.push({ nx - 1, ny });
                checked[left] = true;
            }
            
            if (nx < m_width - 1 && !checked[right] && m_cells[right] == type)
            {
                to_process.push({ nx + 1, ny });
                checked[right] = true;
            }
            
            if (ny > 0 && !checked[bottom] && m_cells[bottom] == type)
            {
                to_process.push({ nx, ny - 1 });
                checked[bottom] = true;
            }
            
            if (ny < m_height - 1 && !checked[top] && m_cells[top] == type)
            {
                to_process.push({ nx, ny + 1 });
                checked[top] = true;
            }
        }

        return tiles;
    };

    for (uint32_t y = 0; y < m_height; ++y)
    for (uint32_t x = 0; x < m_width; ++x)
    {
        uint32_t index = y * m_width + x;
        if (m_cells[index] == type && !checked[index])
            regions.push_back(GetRegion(x, y));
    }

    delete[] checked;
    return regions;
}

void Grid::SetRegion(const std::vector<std::pair<uint32_t, uint32_t>> &region, Cell res)
{
    for (auto[x, y] : region)
        m_cells[y * m_width + x] = res;
}

std::vector<std::pair<uint32_t, uint32_t>> Grid::GetRegionBoundary(const std::vector<std::pair<uint32_t, uint32_t>> &region) const
{
    std::vector<std::pair<uint32_t, uint32_t>> boundary;
    for (auto[x, y] : region)
    {
        Cell c = m_cells[y * m_width + x];
        if ((x > 0 && m_cells[y * m_width + (x - 1)] != c)
            || (x < m_width - 1 && m_cells[y * m_width + (x + 1)] != c)
            || (y > 0 && m_cells[(y - 1) * m_width + x] != c)
            || (y < m_height - 1 && m_cells[(y + 1) * m_width + x] != c))
        {
            boundary.push_back({ x, y });
        }
    }

    return boundary;
}

static float GetPointLineT(float ax, float ay, float bx, float by, float px, float py)
{
    float abx = bx - ax;
    float aby = by - ay;
    float apx = px - ax;
    float apy = py - ay;
    return std::min(std::max((abx * apx + aby * apy) / (abx * abx + aby * aby), 0.0f), 1.0f);
}

void Grid::Process()
{
    // Remove small wall chunks
    auto wall_regions = GetRegions(CELL_WALL);
    uint32_t wall_region_threshold = 50;
    for (auto &region : wall_regions)
    {
        if (region.size() < wall_region_threshold)
            SetRegion(region, CELL_EMPTY);
    }

    // Remove small cave chunks
    auto cave_regions = GetRegions(CELL_EMPTY);
    uint32_t cave_region_threshold = 50;
    for (auto itr = cave_regions.begin(); itr != cave_regions.end();)
    {
        auto region = *itr;
        if (region.size() < cave_region_threshold)
        {
            SetRegion(region, CELL_WALL);
            itr = cave_regions.erase(itr);
        }
        else
        {
            ++itr;
        }
    }

    // Connect regions while there are separate rooms
    float connection_width = 3;
    while (cave_regions.size() > 1)
    {
        auto &r1 = cave_regions[0];
        auto &r2 = cave_regions[1];
        auto b1 = GetRegionBoundary(r1);
        auto b2 = GetRegionBoundary(r2);

        float min_dist_sqr = INFINITY;
        uint32_t min_index_1 = 0;
        uint32_t min_index_2 = 0;

        // Find closest boundary points
        for (uint32_t i = 0; i < b1.size(); ++i)
        for (uint32_t j = 0; j < b2.size(); ++j)
        {
            auto[x1, y1] = b1[i];
            auto[x2, y2] = b2[j];
            float dx = x2 - x1;
            float dy = y2 - y1;

            float dist_sqr = dx * dx + dy * dy;
            if (dist_sqr < min_dist_sqr)
            {
                min_dist_sqr = dist_sqr;
                min_index_1 = i;
                min_index_2 = j;
            }
        }

        auto[x1, y1] = b1[min_index_1];
        auto[x2, y2] = b2[min_index_2];

        // Copy region one over to region two
        r2.insert(r2.end(), r1.begin(), r1.end());

        // Connect both boundary points with capsule
        for (uint32_t x = 0; x < m_width; ++x)
        for (uint32_t y = 0; y < m_height; ++y)
        {
            float t = GetPointLineT(x1, y1, x2, y2, x, y);
            float px = x1 + static_cast<float>(x2 - x1) * t;
            float py = y1 + static_cast<float>(y2 - y1) * t;

            float dx = x - px;
            float dy = y - py;
            float dist_sqr = dx * dx + dy * dy;
            if (dist_sqr < connection_width * connection_width)
            {
                m_cells[y * m_width + x] = CELL_EMPTY;
                if (std::find(r2.begin(), r2.end(), std::pair{ x, y }) == r2.end())
                    r2.push_back({ x, y });
            }
        }

        // Remove region one
        cave_regions.erase(cave_regions.begin());
    }
}