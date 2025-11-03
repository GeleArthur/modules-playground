module;
#include <imgui.h>
#include <iostream>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>


export module EditorApplicationMod;

export class EditorApplication
{
public:
    SDL_AppResult Initialize()
    {
        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

        m_window = SDL_CreateWindow("Editor", 1920, 1080, window_flags);
        m_renderer = SDL_CreateRenderer(m_window, nullptr);

        SDL_SetRenderVSync(m_renderer, 1);

        SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_window);

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        ImGui::StyleColorsDark();

        io.ConfigDpiScaleFonts = true;
        io.ConfigDpiScaleViewports = true;
        ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
        ImGui_ImplSDLRenderer3_Init(m_renderer);

        m_game_Texture = SDL_CreateTexture(m_renderer,
                                           SDL_PIXELFORMAT_RGB24,
                                           SDL_TEXTUREACCESS_TARGET,
                                           1,
                                           1);


        return SDL_APP_CONTINUE;
    }

    void ShowToolPanel()
    {
        ImGui::Begin("Tools");
        ImGui::Text("Tool settings go here.");
        ImGui::End();
    }

    void ShowSceneView()
    {
        ImGui::Begin("Scene");
        ImVec2 size = ImGui::GetContentRegionAvail();

        float width{};
        float height{};
        SDL_GetTextureSize(m_game_Texture, &width, &height);

        if (width != size.x || height != size.y)
        {
            width = size.x;
            height = size.y;
            m_game_Texture = SDL_CreateTexture(m_renderer,
                                               SDL_PIXELFORMAT_RGB24,
                                               SDL_TEXTUREACCESS_TARGET,
                                               width,
                                               height);
        }

        SDL_SetRenderTarget(m_renderer, m_game_Texture);
        ImGui::Image(m_game_Texture, size);

        ImGui::End();
    }

    void ShowInspector()
    {
        ImGui::Begin("Inspector");
        ImGui::Text("Properties panel.");
        ImGui::End();
    }

    void ShowEditorUI()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("DockSpaceWindow", nullptr, window_flags);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New");
                ImGui::MenuItem("Open...");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    SDL_AppResult SDL_AppIterate(void* appstate)
    {
        if (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            return SDL_APP_CONTINUE;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();


        ShowEditorUI();
        ShowToolPanel();
        ShowSceneView();
        ShowInspector();

        SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_renderer);

        SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);

        SDL_FRect rect{
            100, 100, 200, 200
        };
        if (SDL_RenderFillRect(m_renderer, &rect))
        {
            std::cout << SDL_GetError();
        }

        SDL_SetRenderTarget(m_renderer, nullptr);

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
        SDL_RenderPresent(m_renderer);

        return SDL_APP_CONTINUE;
    }

    SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
    {
        ImGui_ImplSDL3_ProcessEvent(event);

        if (event->type == SDL_EVENT_QUIT)
        {
            return SDL_APP_SUCCESS;
        }

        return SDL_APP_CONTINUE;
    }

    void SDL_AppQuit(void* appstate, SDL_AppResult result)
    {
        SDL_DestroyWindow(m_window);
    }

private:
    SDL_Texture* m_game_Texture{};
    SDL_Renderer* m_renderer{};
    SDL_Window* m_window{};
};
