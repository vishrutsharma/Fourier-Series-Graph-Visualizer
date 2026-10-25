#include "Application.h"
#include <cmath>
#include <iostream>
#include <TimeManager.h>

Application::Application() = default;

Application::~Application()
{
    delete m_window;
}

bool Application::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: "
                  << SDL_GetError() << '\n';
        return false;
    }

    m_window = new Window(
        "Fourier Series Visualizer",
        1200,
        800);

    m_running = true;

    m_function.frequency = 4.5f;
    m_function.amplitude = 1.0f;
    m_function.funcType = CC_Graph::HarmonicsType::SIN;
    return true;
}

void Application::run()
{
    while (m_running)
    {
        TimeManager::GetInstance().Tick();
        handleEvents();
        render();
    }
}

void Application::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_dragging = true;
                m_lastMouseX = event.button.x;
                m_lastMouseY = event.button.y;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                m_dragging = false;
            }
            break;

        case SDL_MOUSEMOTION:
            if (m_dragging)
            {
                m_graph.pan(
                    static_cast<float>(
                        event.motion.x - m_lastMouseX),
                    static_cast<float>(
                        event.motion.y - m_lastMouseY));

                m_lastMouseX = event.motion.x;
                m_lastMouseY = event.motion.y;
            }
            break;

        case SDL_MOUSEWHEEL:
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            float factor =
                event.wheel.y > 0 ? 1.1f : 1.0f / 1.1f;

            m_graph.zoom(
                factor,
                static_cast<float>(mouseX),
                static_cast<float>(mouseY));

            break;
        }
        }
    }
}

void Application::render()
{
    int width, height;
    m_window->getSize(width, height);

    SDL_Renderer *renderer = m_window->getRenderer();

    m_window->clear();

    m_graph.drawGrid(renderer, width, height);
    m_graph.drawAxes(renderer, width, height);
    m_graph.drawGraph(renderer, width, m_function);

    m_window->present();
}