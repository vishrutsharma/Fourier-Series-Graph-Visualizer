# Fourier Series Graph Visualizer

A real-time mathematical graphing application built with **C++, SDL2.

The goal of this project is to explore mathematical function visualization, graph rendering, coordinate transformations, and Fourier series while building a modular graphics application from scratch.

The project aims to visualize how individual waves can be combined using Fourier series to approximate more complex periodic signals.

---

## Overview

The application provides a Cartesian coordinate system in which mathematical functions can be visualized and explored.

Its graphing system converts mathematical coordinates into screen coordinates, allowing functions to be rendered correctly regardless of the graph's position or zoom level.

The longer-term goal is to support combining multiple waves and harmonics into a single function, making it possible to observe how changing their parameters affects the resulting waveform.

## Features

### Graphing System

* Cartesian coordinate system with horizontal and vertical axes.
* Mathematical-to-screen coordinate transformations.
* Grid rendering based on the visible coordinate range.
* Panning to explore different regions of the graph.
* Zooming around the mouse cursor.
* Function visualization using SDL2 rendering.

### Wave System

* Represent individual trigonometric waves using configurable parameters.
* Support sine, cosine, and tangent functions.
* Configure amplitude, frequency, phase, and vertical offset.
* Combine multiple waves to calculate a resulting function.

---

## Understanding the Graphing System

A computer screen uses pixel coordinates, whereas mathematical graphs use Cartesian coordinates.

These coordinate systems have different origins and different directions for their vertical axes.

### Screen Coordinates

In SDL2, the screen coordinate system generally follows these rules:

* The origin `(0, 0)` is at the top-left corner.
* The X-coordinate increases toward the right.
* The Y-coordinate increases toward the bottom.

### Mathematical Coordinates

In a conventional Cartesian coordinate system:

* The origin `(0, 0)` is the intersection of the axes.
* Positive X points toward the right.
* Positive Y points upward.

To render a mathematical function correctly, the application must convert between these two coordinate systems.

### Coordinate Transformation

Let:

* `originX` and `originY` represent the screen position of the mathematical origin.
* `scale` represent the number of pixels per mathematical unit.
* `(x, y)` represent a mathematical point.
* `(screenX, screenY)` represent its screen position.

The transformation is:

```cpp
screenX = originX + x * scale;
screenY = originY - y * scale;
```

The subtraction in the Y-coordinate is necessary because screen Y increases downward.

The inverse transformation converts a screen position back into mathematical coordinates:

```cpp
x = (screenX - originX) / scale;
y = (originY - screenY) / scale;
```

These transformations allow the graph to remain mathematically consistent while the user pans or zooms.

### Rendering a Function

For a function such as:

$$
f(x) = \sin(x)
$$

the application samples X-coordinates across the visible screen.

For each screen position, it:

1. Converts the screen X-coordinate into a mathematical X-coordinate.
2. Evaluates the function at that mathematical coordinate.
3. Converts the resulting mathematical point back into screen coordinates.
4. Connects consecutive points using SDL2 drawing operations.

Sampling the function across the screen creates its visual representation.

---

## Understanding Fourier Series

A Fourier series represents a periodic function as a sum of sine and cosine waves.

The central idea is that a sufficiently well-behaved periodic signal can be represented or approximated using combinations of simple trigonometric functions.

Instead of drawing a complicated waveform directly, we can construct it by adding multiple harmonics.

### The General Formula

A Fourier series can be written as:

$$
f(x) = \frac{a_0}{2} +
\sum_{n=1}^{\infty}
\left[
a_n\cos(nx) + b_n\sin(nx)
\right]
$$

Where:

* \(a_0/2\) represents the constant component.
* \(a_n\) represents the cosine coefficient of the nth harmonic.
* \(b_n\) represents the sine coefficient of the nth harmonic.
* \(n\) represents the harmonic number.

In practice, we use a finite number of terms:

$$
f_N(x) = \frac{a_0}{2} +
\sum_{n=1}^{N}
\left[
a_n\cos(nx) + b_n\sin(nx)
\right]
$$

Increasing the number of terms can improve the approximation of many periodic functions, although convergence depends on the function and the chosen series.
