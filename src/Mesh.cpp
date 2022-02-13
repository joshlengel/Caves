#include"Mesh.h"
#include"Grid.h"
#include"Utils.h"

#include<vector>
#include<map>

struct MeshData
{
    GLuint vao;
    GLuint vbo, ibo;
    GLuint program, vs, fs;
};

static const char *const VERTEX_SRC = R"(
    #version 130

    in vec2 position;

    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

static const char *const FRAGMENT_SRC = R"(
    #version 130

    out vec4 color;

    void main() {
        color = vec4(0.05, 0.05, 0.05, 1.0);
    }
)";

Mesh::Mesh(Grid &grid):
    m_data(new MeshData)
{
    // Marching Squares
    struct Node
    {
        float x, y;
    };

    struct Corner : Node
    {
        Node right, above;
        bool active;
    };

    struct Square
    {
        Corner *c1, *c2, *c3, *c4;
        Node *n1, *n2, *n3, *n4;
        uint8_t config;
    };

    Corner *corners = new Corner[grid.GetWidth() * grid.GetHeight()];
    Square *squares = new Square[(grid.GetWidth() - 1) * (grid.GetHeight() - 1)];

    // Initialize corner nodes and center nodes
    uint32_t index = 0;
    for (uint32_t y = 0; y < grid.GetHeight(); ++y)
    for (uint32_t x = 0; x < grid.GetWidth(); ++x)
    {
        Corner &c = corners[index++];
        c.x = static_cast<float>(x) * grid.GetCellSizeX() - 1.0f;
        c.y = static_cast<float>(y) * grid.GetCellSizeY() - 1.0f;
        c.right = { c.x + grid.GetCellSizeX() * 0.5f, c.y };
        c.above = { c.x, c.y + grid.GetCellSizeY() * 0.5f };
        c.active = grid[y][x];
    }

    // Initialize squares
    index = 0;
    for (uint32_t y = 0; y < grid.GetHeight() - 1; ++y)
    for (uint32_t x = 0; x < grid.GetWidth() - 1; ++x)
    {
        Square &s = squares[index];
        s.c1 = &corners[y * grid.GetWidth() + x];
        s.c2 = &corners[y * grid.GetWidth() + (x + 1)];
        s.c3 = &corners[(y + 1) * grid.GetWidth() + x];
        s.c4 = &corners[(y + 1) * grid.GetWidth() + (x + 1)];
        s.n1 = &s.c1->right;
        s.n2 = &s.c2->above;
        s.n3 = &s.c3->right;
        s.n4 = &s.c1->above;
        s.config = s.c1->active | s.c2->active << 1 | s.c3->active << 2 | s.c4->active << 3;

        ++index;
    }

    std::vector<Node> positions;
    std::vector<unsigned int> indices;
    std::map<Node*, unsigned int> nodes;

    // Looks up node. Adds if it doesn't exist yet otherwise returns existing one
    auto GetNodeIndex = [&](Node *n)
    {
        auto itr = nodes.find(n);
        if (itr == nodes.end())
        {
            // Add node to list
            unsigned int index = positions.size();
            nodes[n] = index;
            positions.push_back(*n);
            return index;
        }
        else
        {
            return itr->second;
        }
    };

    auto AddTriangle = [&](Node *n1, Node *n2, Node *n3)
    {
        indices.push_back(GetNodeIndex(n1));
        indices.push_back(GetNodeIndex(n2));
        indices.push_back(GetNodeIndex(n3));
    };

    // Create mesh from each square
    index = 0;
    for (uint32_t y = 0; y < grid.GetHeight() - 1; ++y)
    for (uint32_t x = 0; x < grid.GetWidth() - 1; ++x)
    {
        Square &s = squares[index++];
        switch (s.config)
        {
        // 0-point cases
            case 0:
                break;
        
        // 1-point cases
            case 1:
                AddTriangle(s.c1, s.n1, s.n4);
                break;
            
            case 2:
                AddTriangle(s.c2, s.n2, s.n1);
                break;
            
            case 4:
                AddTriangle(s.c3, s.n4, s.n3);
                break;
            
            case 8:
                AddTriangle(s.c4, s.n3, s.n2);
                break;
            
        // 2-point cases
            case 3:
                AddTriangle(s.c1, s.n2, s.n4);
                AddTriangle(s.c1, s.c2, s.n2);
                break;
            
            case 10:
                AddTriangle(s.c2, s.n3, s.n1);
                AddTriangle(s.c2, s.c4, s.n3);
                break;
            
            case 12:
                AddTriangle(s.c3, s.n4, s.n2);
                AddTriangle(s.c3, s.n2, s.c4);
                break;
            
            case 5:
                AddTriangle(s.c3, s.c1, s.n1);
                AddTriangle(s.c3, s.n1, s.n3);
                break;
            
            case 9:
                AddTriangle(s.c1, s.n1, s.n2);
                AddTriangle(s.c1, s.n2, s.c4);
                AddTriangle(s.c1, s.c4, s.n3);
                AddTriangle(s.c1, s.n3, s.n4);
                break;
            
            case 6:
                AddTriangle(s.c2, s.n2, s.n3);
                AddTriangle(s.c2, s.n3, s.c3);
                AddTriangle(s.c2, s.c3, s.n4);
                AddTriangle(s.c2, s.n4, s.n1);
                break;
            
        // 3-point cases
            case 11:
                AddTriangle(s.c1, s.c4, s.n3);
                AddTriangle(s.c1, s.n3, s.n4);
                AddTriangle(s.c1, s.c2, s.c4);
                break;
            
            case 14:
                AddTriangle(s.c2, s.c3, s.n4);
                AddTriangle(s.c2, s.n4, s.n1);
                AddTriangle(s.c2, s.c4, s.c3);
                break;
            
            case 13:
                AddTriangle(s.c1, s.n1, s.n2);
                AddTriangle(s.c1, s.n2, s.c4);
                AddTriangle(s.c1, s.c4, s.c3);
                break;
            
            case 7:
                AddTriangle(s.c3, s.c2, s.n2);
                AddTriangle(s.c3, s.n2, s.n3);
                AddTriangle(s.c1, s.c2, s.c3);
                break;
        
        // 4-point cases
            case 15:
                AddTriangle(s.c1, s.c2, s.c4);
                AddTriangle(s.c1, s.c4, s.c3);
                break;
        }
    }

    m_index_count = indices.size();

    delete[] corners;
    delete[] squares;

    InitializeBuffers2D(m_data->vao, m_data->vbo, m_data->ibo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Node), positions.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    InitializeShaders(m_data->program, m_data->vs, m_data->fs, VERTEX_SRC, FRAGMENT_SRC);
}

Mesh::~Mesh()
{
    DestroyBuffers(m_data->vao, m_data->vbo, m_data->ibo);
    DestroyShaders(m_data->program, m_data->vs, m_data->fs);
    delete m_data;
}

void Mesh::Render()
{
    glUseProgram(m_data->program);
    glBindVertexArray(m_data->vao);
    glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, nullptr);
}