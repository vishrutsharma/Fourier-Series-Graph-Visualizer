#include "Graph.h"
#include <algorithm>
#include <cmath>

Graph::Graph(float originX, float originY, float scale)
    : m_originX(originX),
      m_originY(originY),
      m_scale(scale)
{
}

SDL_FPoint Graph::toScreen(float x, float y) const
{
    return {
        m_originX + x * m_scale,
        m_originY - y * m_scale};
}

SDL_FPoint Graph::toMath(float screenX, float screenY) const
{
    return {
        (screenX - m_originX) / m_scale,
        (m_originY - screenY) / m_scale};
}

void Graph::pan(float deltaX, float deltaY)
{
    m_originX += deltaX;
    m_originY += deltaY;
}

void Graph::zoom(float factor, float mouseX, float mouseY)
{
    SDL_FPoint pointBeforeZoom = toMath(mouseX, mouseY);

    m_scale = std::clamp(m_scale * factor, 5.0f, 300.0f);

    m_originX = mouseX - pointBeforeZoom.x * m_scale;
    m_originY = mouseY + pointBeforeZoom.y * m_scale;
}

void Graph::drawGrid(SDL_Renderer *renderer, int width, int height) const
{
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);

    int startX = static_cast<int>(std::floor(-m_originX / m_scale));

    int endX = static_cast<int>(std::ceil((width - m_originX) / m_scale));

    for (int x = startX; x <= endX; ++x)
    {
        SDL_FPoint point = toScreen(x, 0);

        SDL_RenderDrawLineF(renderer, point.x, 0, point.x, static_cast<float>(height));
    }

    int startY = static_cast<int>(std::floor((m_originY - height) / m_scale));
    int endY = static_cast<int>(std::ceil(m_originY / m_scale));

    for (int y = startY; y <= endY; ++y)
    {
        SDL_FPoint point = toScreen(0, y);

        SDL_RenderDrawLineF(renderer, 0, point.y, static_cast<float>(width), point.y);
    }
}

void Graph::drawAxes(SDL_Renderer *renderer, int width, int height) const
{
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderDrawLineF(renderer, 0, m_originY, static_cast<float>(width), m_originY);
    SDL_RenderDrawLineF(renderer, m_originX, 0, m_originX, static_cast<float>(height));
}

void Graph::drawGraph(SDL_Renderer *renderer, int width, CC_Graph::Harmonics &harmonic) const
{
    SDL_SetRenderDrawColor(renderer, 0, 220, 150, 255);

    SDL_FPoint previous{};
    bool hasPrevious = false;

    for (int pixelX = 0; pixelX < width; ++pixelX)
    {
        SDL_FPoint mathPoint = toMath(static_cast<float>(pixelX), 0);
        float y = harmonic.Draw(mathPoint.x);
        SDL_FPoint current = toScreen(mathPoint.x, y);

        if (hasPrevious)
        {
            SDL_RenderDrawLineF(renderer, previous.x, previous.y, current.x, current.y);
        }

        previous = current;
        hasPrevious = true;
    }
}