#pragma once
#include <SDL2/SDL.h>
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

    void drawGraph(
        SDL_Renderer *renderer, int width, CC_Graph::Harmonics &function) const;

private:
    float m_originX;
    float m_originY;
    float m_scale;
};