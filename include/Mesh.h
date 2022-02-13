#pragma once

#include<cstdint>

struct MeshData;
class Grid;

/**
 * @brief Takes the grid and generates a mesh encapsulating all of the wall cells smoothly
 * using marching squares.
 * 
 */
class Mesh
{
public:
    Mesh(Grid &grid);
    ~Mesh();

    /**
     * @brief Renders the OpenGL mesh generated in the constructor.
     * 
     */
    void Render();

private:
    MeshData *m_data;
    uint32_t m_index_count;
};