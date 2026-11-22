#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <memory>
#include <vector>
#include "CoreComponents.h"

class Graph
{
public:
    Graph(float originX, float originY, float scale);

    SDL_FPoint toScreen(float x, float y) const;
    SDL_FPoint toMath(float screenX, float screenY) const;

    void pan(float deltaX, float deltaY);
    void zoom(float factor, float mouseX, float mouseY);

    void setMode(CC_Graph::CoordinateMode mode) { m_mode = mode; }
    CC_Graph::CoordinateMode getMode() const { return m_mode; }
    void setPolarTurns(float turns) { m_polarTurns = turns; }
    float getPolarTurns() const { return m_polarTurns; }

    void drawGrid(
        SDL_Renderer *renderer,
        int width,
        int height) const;

    void drawAxes(
        SDL_Renderer *renderer,
        int width,
        int height) const;

    void drawGraph(
        SDL_Renderer *renderer, int width, const CC_Graph::Harmonics &harmonic) const;

    void drawSum(
        SDL_Renderer *renderer, int width,
        const std::vector<std::unique_ptr<CC_Graph::Harmonics>> &harmonics,
        SDL_Color color) const;

    void drawFunction(
        SDL_Renderer *renderer, int width,
        const std::function<float(float)> &function, SDL_Color color) const;

private:
    void drawCartesianGrid(SDL_Renderer *renderer, int width, int height) const;
    void drawPolarGrid(SDL_Renderer *renderer, int width, int height) const;

    void drawCartesianFunction(
        SDL_Renderer *renderer, int width,
        const std::function<float(float)> &function) const;
    void drawPolarFunction(
        SDL_Renderer *renderer,
        const std::function<float(float)> &function) const;

    CC_Graph::CoordinateMode m_mode = CC_Graph::CoordinateMode::CARTESIAN;
    float m_polarTurns = 1.0f;

    float m_originX;
    float m_originY;
    float m_scale;
};