#pragma once

#include<cstdint>

struct MeshData;
class Grid;

class Mesh
{
public:
    Mesh(Grid &grid);
    ~Mesh();

    void Render();

private:
    MeshData *m_data;
    uint32_t m_index_count;
};