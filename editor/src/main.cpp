#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <cstdio>
#include <string>
#include <vector>

#ifndef LHE_VERSION
#define LHE_VERSION "0.1.0"
#endif

namespace
{
struct Vec3
{
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;
};

struct SceneObject
{
    std::string name;
    std::string type;
    Vec3 position{};
    Vec3 rotation{};
    Vec3 scale{1.0F, 1.0F, 1.0F};
    bool active = true;
};

std::vector<SceneObject> g_objects{
    {"Camera", "Camera"},
    {"Directional Light", "Light"},
    {"Cube", "Mesh"}
};
int g_selectedObject = 2;
bool g_showAbout = false;
bool g_showImGuiDemo = false;
bool g_showMetrics = false;
bool g_showGrid = true;

void DrawVector3(const char* label, Vec3& value, float resetValue)
{
    ImGui::PushID(label);
    ImGui::TextUnformatted(label);
    ImGui::SameLine(112.0F);

    const float available = ImGui::GetContentRegionAvail().x;
    const float width = (available - 12.0F) / 3.0F;
    ImGui::SetNextItemWidth(width);
    ImGui::DragFloat("##x", &value.x, 0.05F, 0.0F, 0.0F, "X %.3f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(width);
    ImGui::DragFloat("##y", &value.y, 0.05F, 0.0F, 0.0F, "Y %.3f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(width);
    ImGui::DragFloat("##z", &value.z, 0.05F, 0.0F, 0.0F, "Z %.3f");

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        value = {resetValue, resetValue, resetValue};

    ImGui::PopID();
}

void DrawMenuBar(bool& running)
{
    if (!ImGui::BeginMainMenuBar())
        return;

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene", "Ctrl+N"))
        {
            g_objects.clear();
            g_selectedObject = -1;
        }
        if (ImGui::MenuItem("Add Cube"))
        {
            g_objects.push_back({"Cube", "Mesh"});
            g_selectedObject = static_cast<int>(g_objects.size()) - 1;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
            running = false;
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        ImGui::MenuItem("ImGui Demo", nullptr, &g_showImGuiDemo);
        ImGui::MenuItem("Metrics", nullptr, &g_showMetrics);
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("About Little Hous Engine"))
            g_showAbout = true;
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void DrawHierarchy()
{
    ImGui::Begin("Hierarchy");
    ImGui::TextDisabled("%zu objects", g_objects.size());
    ImGui::Separator();

    for (int i = 0; i < static_cast<int>(g_objects.size()); ++i)
    {
        ImGui::PushID(i);
        const bool selected = (i == g_selectedObject);
        if (ImGui::Selectable(g_objects[static_cast<std::size_t>(i)].name.c_str(), selected))
            g_selectedObject = i;
        ImGui::PopID();
    }

    if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight))
    {
        if (ImGui::MenuItem("Create Cube"))
        {
            g_objects.push_back({"Cube", "Mesh"});
            g_selectedObject = static_cast<int>(g_objects.size()) - 1;
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("+ Add Cube"))
    {
        g_objects.push_back({"Cube", "Mesh"});
        g_selectedObject = static_cast<int>(g_objects.size()) - 1;
    }
    ImGui::End();
}

void DrawInspector()
{
    ImGui::Begin("Inspector");

    if (g_selectedObject < 0 || g_selectedObject >= static_cast<int>(g_objects.size()))
    {
        ImGui::TextDisabled("Select an object in the Hierarchy.");
        ImGui::End();
        return;
    }

    SceneObject& object = g_objects[static_cast<std::size_t>(g_selectedObject)];
    ImGui::Checkbox("Active", &object.active);
    ImGui::SameLine();
    ImGui::TextDisabled("%s", object.type.c_str());
    ImGui::Separator();

    char nameBuffer[256]{};
    std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", object.name.c_str());
    if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
        object.name = nameBuffer;

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawVector3("Position", object.position, 0.0F);
        DrawVector3("Rotation", object.rotation, 0.0F);
        DrawVector3("Scale", object.scale, 1.0F);
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Inspector edits the in-memory scene only.");
    ImGui::End();
}

void DrawViewport()
{
    ImGui::Begin("Scene View");
    ImGui::TextDisabled("Viewport placeholder");
    ImGui::SameLine();
    ImGui::Checkbox("Grid", &g_showGrid);
    ImGui::Separator();

    const ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::InvisibleButton("ViewportSurface", size, ImGuiButtonFlags_MouseButtonLeft);
    const ImVec2 min = ImGui::GetItemRectMin();
    const ImVec2 max = ImGui::GetItemRectMax();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    draw->AddRectFilled(min, max, IM_COL32(22, 27, 34, 255));

    if (g_showGrid)
    {
        constexpr float spacing = 32.0F;
        for (float x = min.x; x < max.x; x += spacing)
            draw->AddLine({x, min.y}, {x, max.y}, IM_COL32(43, 51, 61, 255));
        for (float y = min.y; y < max.y; y += spacing)
            draw->AddLine({min.x, y}, {max.x, y}, IM_COL32(43, 51, 61, 255));
    }

    const ImVec2 center{(min.x + max.x) * 0.5F, (min.y + max.y) * 0.5F};
    draw->AddCircle(center, 26.0F, IM_COL32(95, 156, 225, 255), 0, 2.0F);
    draw->AddLine({center.x - 42.0F, center.y}, {center.x + 42.0F, center.y},
                  IM_COL32(95, 156, 225, 255), 2.0F);
    draw->AddLine({center.x, center.y - 42.0F}, {center.x, center.y + 42.0F},
                  IM_COL32(225, 104, 104, 255), 2.0F);
    draw->AddText({min.x + 12.0F, max.y - 26.0F}, IM_COL32(180, 190, 203, 255),
                  "No 3D renderer connected yet");
    ImGui::End();
}

void DrawContentBrowser()
{
    ImGui::Begin("Content Browser");
    ImGui::TextDisabled("Project assets");
    ImGui::Separator();
    ImGui::TextWrapped("Asset discovery and importing are not implemented yet.");
    ImGui::TextDisabled("Project root: (not configured)");
    ImGui::End();
}

void DrawAbout()
{
    if (!g_showAbout)
        return;

    if (ImGui::Begin("About Little Hous Engine", &g_showAbout))
    {
        ImGui::Text("Little Hous Engine %s", LHE_VERSION);
        ImGui::Text("An independent engine project by Little Hous Group.");
        ImGui::Separator();
        ImGui::TextWrapped("This early editor shell provides a desktop window, dockable panels, and an in-memory scene hierarchy. Rendering, physics, saving, and importing are not implemented yet.");
    }
    ImGui::End();
}
} // namespace

int main(int, char**)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow(
        "Little Hous Engine", 1440, 900,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );

    if (window == nullptr)
    {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!SDL_GL_MakeCurrent(window, glContext))
    {
        std::fprintf(stderr, "SDL_GL_MakeCurrent failed: %s\n", SDL_GetError());
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0F;
    style.FrameRounding = 4.0F;
    style.GrabRounding = 4.0F;
    style.WindowPadding = ImVec2(10.0F, 10.0F);
    style.FramePadding = ImVec2(8.0F, 5.0F);

    if (!ImGui_ImplSDL3_InitForOpenGL(window, glContext) ||
        !ImGui_ImplOpenGL3_Init("#version 330"))
    {
        std::fprintf(stderr, "Dear ImGui backend initialization failed.\n");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                event.window.windowID == SDL_GetWindowID(window))
                running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        DrawMenuBar(running);
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        DrawHierarchy();
        DrawInspector();
        DrawViewport();
        DrawContentBrowser();
        DrawAbout();

        if (g_showImGuiDemo)
            ImGui::ShowDemoWindow(&g_showImGuiDemo);
        if (g_showMetrics)
            ImGui::ShowMetricsWindow(&g_showMetrics);

        ImGui::Render();
        int width = 0;
        int height = 0;
        SDL_GetWindowSizeInPixels(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.055F, 0.065F, 0.080F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
