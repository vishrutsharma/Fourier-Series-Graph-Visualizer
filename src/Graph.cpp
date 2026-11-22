#include "Graph.h"
#include <algorithm>
#include <cmath>

namespace
{
    constexpr float kPi = 3.14159265358979f;
    constexpr float kTwoPi = 2.0f * kPi;
}

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
    if (m_mode == CC_Graph::CoordinateMode::POLAR)
        drawPolarGrid(renderer, width, height);
    else
        drawCartesianGrid(renderer, width, height);
}

void Graph::drawCartesianGrid(SDL_Renderer *renderer, int width, int height) const
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

void Graph::drawPolarGrid(SDL_Renderer *renderer, int width, int height) const
{
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);

    // Furthest screen corner from the origin, in math units, so the rings
    // always cover the whole window.
    float maxDist = 0.0f;
    const float corners[4][2] = {{0, 0}, {(float)width, 0}, {0, (float)height}, {(float)width, (float)height}};
    for (const auto &c : corners)
    {
        float dx = c[0] - m_originX;
        float dy = c[1] - m_originY;
        maxDist = std::max(maxDist, std::sqrt(dx * dx + dy * dy));
    }
    int maxRadius = static_cast<int>(std::ceil(maxDist / m_scale));

    // Concentric circles at every integer radius.
    for (int r = 1; r <= maxRadius; ++r)
    {
        float screenR = r * m_scale;
        int segments = std::clamp(static_cast<int>(screenR), 32, 720);
        SDL_FPoint prev{m_originX + screenR, m_originY};
        for (int i = 1; i <= segments; ++i)
        {
            float a = kTwoPi * i / segments;
            SDL_FPoint cur{m_originX + screenR * std::cos(a), m_originY - screenR * std::sin(a)};
            SDL_RenderDrawLineF(renderer, prev.x, prev.y, cur.x, cur.y);
            prev = cur;
        }
    }

    // Radial spokes every 30 degrees.
    float spokeLen = maxRadius * m_scale;
    for (int deg = 0; deg < 360; deg += 30)
    {
        float a = deg * kPi / 180.0f;
        SDL_RenderDrawLineF(renderer, m_originX, m_originY,
                            m_originX + spokeLen * std::cos(a),
                            m_originY - spokeLen * std::sin(a));
    }
}

void Graph::drawAxes(SDL_Renderer *renderer, int width, int height) const
{
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderDrawLineF(renderer, 0, m_originY, static_cast<float>(width), m_originY);
    SDL_RenderDrawLineF(renderer, m_originX, 0, m_originX, static_cast<float>(height));
}

void Graph::drawGraph(SDL_Renderer *renderer, int width, const CC_Graph::Harmonics &harmonic) const
{
    drawFunction(
        renderer, width,
        [&harmonic](float x)
        { return harmonic.Draw(x); },
        harmonic.color);
}

void Graph::drawSum(
    SDL_Renderer *renderer, int width,
    const std::vector<std::unique_ptr<CC_Graph::Harmonics>> &harmonics,
    SDL_Color color) const
{
    drawFunction(
        renderer, width,
        [&harmonics](float x)
        {
            float sum = 0.0f;
            for (const auto &harmonic : harmonics)
            {
                sum += harmonic->Draw(x);
            }
            return sum;
        },
        color);
}

void Graph::drawFunction(
    SDL_Renderer *renderer, int width,
    const std::function<float(float)> &function, SDL_Color color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (m_mode == CC_Graph::CoordinateMode::POLAR)
        drawPolarFunction(renderer, function);
    else
        drawCartesianFunction(renderer, width, function);
}

void Graph::drawCartesianFunction(
    SDL_Renderer *renderer, int width,
    const std::function<float(float)> &function) const
{
    SDL_FPoint previous{};
    bool hasPrevious = false;

    for (int pixelX = 0; pixelX < width; ++pixelX)
    {
        SDL_FPoint mathPoint = toMath(static_cast<float>(pixelX), 0);
        float y = function(mathPoint.x);
        SDL_FPoint current = toScreen(mathPoint.x, y);

        if (hasPrevious)
        {
            SDL_RenderDrawLineF(renderer, previous.x, previous.y, current.x, current.y);
        }

        previous = current;
        hasPrevious = true;
    }
}

void Graph::drawPolarFunction(
    SDL_Renderer *renderer,
    const std::function<float(float)> &function) const
{
    // Treat the function input as theta and its output as the radius r,
    // then convert (r, theta) to (x, y) = (r cos theta, r sin theta).
    constexpr int kSamplesPerTurn = 2000;
    constexpr float kMaxRadius = 1000.0f; // skip blow-ups (e.g. tan)

    const float thetaEnd = kTwoPi * m_polarTurns;
    const int samples = std::max(2, static_cast<int>(kSamplesPerTurn * m_polarTurns));

    SDL_FPoint previous{};
    bool hasPrevious = false;

    for (int i = 0; i <= samples; ++i)
    {
        float theta = thetaEnd * i / samples;
        float r = function(theta);

        if (!std::isfinite(r) || std::fabs(r) > kMaxRadius)
        {
            hasPrevious = false;
            continue;
        }

        SDL_FPoint current = toScreen(r * std::cos(theta), r * std::sin(theta));

        if (hasPrevious)
        {
            SDL_RenderDrawLineF(renderer, previous.x, previous.y, current.x, current.y);
        }

        previous = current;
        hasPrevious = true;
    }
}
