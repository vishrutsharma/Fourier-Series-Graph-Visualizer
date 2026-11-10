#pragma once

#include <memory>

#include "Window.h"
#include "Graph.h"
#include "CoreComponents.h"
#include "WaveComposer.h"

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
    void renderUI();
    void createRandomHarmonics(int count);

    std::unique_ptr<Window> m_window;
    Graph m_graph{600.0f, 400.0f, 50.0f};

    bool m_running = false;
    bool m_imguiInitialized = false;
    bool m_showSum = true;
    bool m_dragging = false;

    int m_lastMouseX = 0;
    int m_lastMouseY = 0;
    WaveComposer m_composer;
};