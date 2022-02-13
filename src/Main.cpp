#include"Window.h"
#include"Grid.h"
#include"Automata.h"

#include<glad/glad.h>

#include<stdexcept>

static const uint32_t WIDTH = 900;
static const uint32_t HEIGHT = 600;
static const char *const TITLE = "Marching Squares";

static const uint32_t CELL_SIZE = 10;
static const uint32_t GRID_WIDTH = WIDTH / CELL_SIZE;
static const uint32_t GRID_HEIGHT = HEIGHT / CELL_SIZE;
static const float CELL_SIZE_X = 2.0f / static_cast<float>(GRID_WIDTH);
static const float CELL_SIZE_Y = 2.0f / static_cast<float>(GRID_HEIGHT);

static const uint32_t STEPS_PER_SECOND = 10;

int main()
{
    Window window(WIDTH, HEIGHT, TITLE);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    srand(time(nullptr));
    Grid grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE_X, CELL_SIZE_Y, 0.45f);
    Automaton automaton(grid);
    for (uint32_t i = 0; i < 10; ++i)
        automaton.Step();

    window.Show();

    while (!window.ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        grid.Render();

        window.Update();
    }
}