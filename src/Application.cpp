#include "Application.h"
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include "TimeManager.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

Application::Application() = default;

Application::~Application()
{
    if (m_imguiInitialized)
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
}

bool Application::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: "
                  << SDL_GetError() << '\n';
        return false;
    }

    m_window = std::make_unique<Window>(
        "Fourier Series Visualizer",
        1200,
        1080);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(m_window->getWindow(), m_window->getRenderer());
    ImGui_ImplSDLRenderer2_Init(m_window->getRenderer());
    m_imguiInitialized = true;

    m_running = true;

    createRandomHarmonics(4);
    return true;
}

void Application::createRandomHarmonics(int count)
{
    std::mt19937 rng{std::random_device{}()};

    std::uniform_int_distribution<int> typeDist(0, 1); // 0 = SIN, 1 = COS
    std::uniform_real_distribution<float> amplitudeDist(0.5f, 2.0f);
    std::uniform_real_distribution<float> frequencyDist(0.5f, 5.0f);
    std::uniform_real_distribution<float> phaseDist(0.0f, 6.2831853f);
    std::uniform_int_distribution<int> colorDist(80, 255);

    for (int i = 0; i < count; ++i)
    {
        auto harmonic = std::make_unique<CC_Graph::Harmonics>();

        harmonic->funcType = typeDist(rng) == 0
                                 ? CC_Graph::HarmonicsType::SIN
                                 : CC_Graph::HarmonicsType::COS;
        harmonic->amplitude = amplitudeDist(rng);
        harmonic->frequency = frequencyDist(rng);
        harmonic->phase = phaseDist(rng);
        harmonic->color = {
            static_cast<Uint8>(colorDist(rng)),
            static_cast<Uint8>(colorDist(rng)),
            static_cast<Uint8>(colorDist(rng)),
            255};

        m_composer.AddHarmonic(std::move(harmonic));
    }
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
        ImGui_ImplSDL2_ProcessEvent(&event);
        const bool imguiWantsMouse = ImGui::GetIO().WantCaptureMouse;

        switch (event.type)
        {
        case SDL_QUIT:
            m_running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT && !imguiWantsMouse)
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
            if (imguiWantsMouse)
                break;

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
    for (const auto &harmonic : m_composer.GetHarmonics())
    {
        m_graph.drawGraph(renderer, width, *harmonic);
    }

    if (m_showSum && !m_composer.GetHarmonics().empty())
    {
        m_graph.drawSum(
            renderer, width, m_composer.GetHarmonics(),
            SDL_Color{255, 255, 255, 255});
    }

    renderUI();

    m_window->present();
}

void Application::renderUI()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(320, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("Harmonics");

    ImGui::Text("FPS: %.1f", TimeManager::GetInstance().GetFPS());
    ImGui::Checkbox("Show sum (white)", &m_showSum);

    if (ImGui::Button("Add Harmonic"))
    {
        createRandomHarmonics(1);
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear All"))
    {
        m_composer.Clear();
    }

    ImGui::Separator();

    auto &harmonics = m_composer.GetHarmonics();
    int indexToRemove = -1;

    static const char *typeNames[] = {"None", "Sin", "Cos", "Tan"};

    for (int i = 0; i < static_cast<int>(harmonics.size()); ++i)
    {
        CC_Graph::Harmonics &harmonic = *harmonics[i];

        ImGui::PushID(i);
        ImVec4 swatch(harmonic.color.r / 255.0f, harmonic.color.g / 255.0f,
                      harmonic.color.b / 255.0f, 1.0f);
        ImGui::ColorButton("##swatch", swatch, ImGuiColorEditFlags_NoTooltip, ImVec2(14, 14));
        ImGui::SameLine();

        if (ImGui::CollapsingHeader(("Harmonic " + std::to_string(i + 1)).c_str(),
                                    ImGuiTreeNodeFlags_DefaultOpen))
        {
            int type = static_cast<int>(harmonic.funcType);
            if (ImGui::Combo("Type", &type, typeNames, IM_ARRAYSIZE(typeNames)))
            {
                harmonic.funcType = static_cast<CC_Graph::HarmonicsType>(type);
            }

            ImGui::DragFloat("Amplitude", &harmonic.amplitude, 0.01f, -10.0f, 10.0f);
            ImGui::DragFloat("Frequency", &harmonic.frequency, 0.01f, 0.0f, 20.0f);
            ImGui::SliderAngle("Phase", &harmonic.phase, 0.0f, 360.0f);
            ImGui::DragFloat("Offset", &harmonic.offset, 0.01f, -10.0f, 10.0f);

            float color[3] = {harmonic.color.r / 255.0f,
                              harmonic.color.g / 255.0f,
                              harmonic.color.b / 255.0f};
            if (ImGui::ColorEdit3("Color", color))
            {
                harmonic.color.r = static_cast<Uint8>(color[0] * 255.0f);
                harmonic.color.g = static_cast<Uint8>(color[1] * 255.0f);
                harmonic.color.b = static_cast<Uint8>(color[2] * 255.0f);
            }

            if (ImGui::Button("Remove"))
            {
                indexToRemove = i;
            }

            ImGui::Spacing();
        }

        ImGui::PopID();
    }

    if (indexToRemove >= 0)
    {
        m_composer.RemoveHarmonic(static_cast<std::size_t>(indexToRemove));
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_window->getRenderer());
}