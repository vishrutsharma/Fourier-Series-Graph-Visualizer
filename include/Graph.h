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

    void drawGrid(
        SDL_Renderer *renderer,
        int width,
        int height) const;

    void drawAxes(
        SDL_Renderer *renderer,
        int width,
        int height) const;

    // Draws a single harmonic in its own color.
    void drawGraph(
        SDL_Renderer *renderer, int width, const CC_Graph::Harmonics &harmonic) const;

    // Draws the sum of all harmonics (the composed wave) in the given color.
    void drawSum(
        SDL_Renderer *renderer, int width,
        const std::vector<std::unique_ptr<CC_Graph::Harmonics>> &harmonics,
        SDL_Color color) const;

    // Draws any y = f(x) function in the given color.
    void drawFunction(
        SDL_Renderer *renderer, int width,
        const std::function<float(float)> &function, SDL_Color color) const;

private:
    float m_originX;
    float m_originY;
    float m_scale;
};