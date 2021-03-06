#include"Window.h"
#include"Grid.h"
#include"Automata.h"
#include"Mesh.h"

#include<glad/glad.h>

#include<stdexcept>

// Window parameters
static const uint32_t WIDTH = 900;
static const uint32_t HEIGHT = 600;
static const char *const TITLE = "Marching Squares";

// Grid parameters
static const uint32_t CELL_SIZE = 10;
static const uint32_t GRID_WIDTH = WIDTH / CELL_SIZE + 1;
static const uint32_t GRID_HEIGHT = HEIGHT / CELL_SIZE + 1;
static const float CELL_SIZE_X = 2.0f / static_cast<float>(GRID_WIDTH - 1);
static const float CELL_SIZE_Y = 2.0f / static_cast<float>(GRID_HEIGHT - 1);

int main()
{
    // Create window
    Window window(WIDTH, HEIGHT, TITLE);

    // Set background (empty cells) to brown
    glClearColor(0.3f, 0.2f, 0.1f, 1.0f);

    // Create random grid
    srand(time(nullptr));
    Grid grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE_X, CELL_SIZE_Y, 0.45f);

    // Smooth it out using cellular automaton
    Automaton automaton(grid);
    for (uint32_t i = 0; i < 10; ++i)
        automaton.Step();

    // Add finishing touches
    grid.Process();
    
    // Generate mesh with marching squares
    Mesh mesh(grid);

    // Start display loop
    window.Show();

    while (!window.ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        mesh.Render();

        window.Update();
    }
}