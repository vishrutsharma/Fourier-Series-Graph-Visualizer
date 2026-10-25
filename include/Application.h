#pragma once

#include "Window.h"
#include "Graph.h"
#include "CoreComponents.h"

class Application
{
public:
    Application();
    ~Application();

    bool initialize();
    void run();

private:
    void handleEvents();
    void render();

    Window *m_window = nullptr;
    Graph m_graph{600.0f, 400.0f, 50.0f};

    bool m_running = false;
    bool m_dragging = false;

    int m_lastMouseX = 0;
    int m_lastMouseY = 0;
    CC_Graph::Harmonics m_function{};
};