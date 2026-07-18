#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "ImGuiFileDialog.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "cad_model.h"
#include <iostream>
#include "implot.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void setup_fonts()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    
    const char* paths[] = {
        "external/imgui/misc/fonts/DroidSans.ttf",
        "../external/imgui/misc/fonts/DroidSans.ttf",
        "../../external/imgui/misc/fonts/DroidSans.ttf"
    };
    
    ImFont* font = nullptr;
    for (int i = 0; i < 3; i++) {
        FILE* f = fopen(paths[i], "rb");
        if (f) {
            fclose(f);
            font = io.Fonts->AddFontFromFileTTF(paths[i], 22.0f);
            break;
        }
    }
    
    if (!font) {
        io.Fonts->AddFontDefault();
        font = io.Fonts->Fonts.back();
        font->Scale = 1.8f;
    }
}

void show_error_dialog(const char* errorMessage) {
    ImGui::OpenPopup("Error");

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
            ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("%s", errorMessage);

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus(); 
        ImGui::EndPopup();
    }
}

static bool show_about = false;
static ImVec4 clear_color = ImVec4(0.09f, 0.09f, 0.10f, 1.00f); // Match dashboard background
static bool show_demo_window = false;

static void show_about_dialog() {
    static bool open = true;
    if (ImGui::BeginPopupModal("About Myapp", &open, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("imguiplay - CAD Geometry Inspector & Test bed");
        ImGui::Separator();

        ImGui::Text("Version: 2.0.0");
        ImGui::Text("Author: skyera");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("License");
        ImGui::Text("This application is licensed under the MIT License.");

        ImGui::Spacing();
        if (ImGui::Button("Visit GitHub", ImVec2(250, 30))) {
#ifdef _WIN32
            system("start https://github.com/skeyera/imguiplay");
#elif __APPLE__
            system("open https://github.com/skyera/imguiplay");
#else
            system("xdg-open https://github.com/skyera/imguiplay");
#endif
        }

        ImGui::Spacing();
        if (ImGui::Button("Close", ImVec2(250, 30))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void apply_premium_theme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;

    // Dark charcoal background
    colors[ImGuiCol_WindowBg]             = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]              = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_PopupBg]              = ImVec4(0.12f, 0.12f, 0.14f, 0.98f);
    colors[ImGuiCol_Border]               = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Frame backgrounds
    colors[ImGuiCol_FrameBg]              = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.24f, 0.24f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);

    // Titles
    colors[ImGuiCol_TitleBg]              = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);

    // Menus
    colors[ImGuiCol_MenuBarBg]            = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab]                  = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered]           = ImVec4(0.28f, 0.23f, 0.54f, 0.80f); // Sleek Indigo accent
    colors[ImGuiCol_TabActive]            = ImVec4(0.35f, 0.29f, 0.68f, 1.00f); // Bright Indigo accent
    colors[ImGuiCol_TabUnfocused]         = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);

    // Buttons
    colors[ImGuiCol_Button]               = ImVec4(0.35f, 0.29f, 0.68f, 1.00f); // Indigo button
    colors[ImGuiCol_ButtonHovered]        = ImVec4(0.43f, 0.36f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonActive]         = ImVec4(0.28f, 0.23f, 0.54f, 1.00f);

    // Headers (table headers, collapsing headers)
    colors[ImGuiCol_Header]               = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered]        = ImVec4(0.28f, 0.23f, 0.54f, 0.80f);
    colors[ImGuiCol_HeaderActive]         = ImVec4(0.35f, 0.29f, 0.68f, 1.00f);

    // Active components (Checkboxes, sliders, progress bars)
    colors[ImGuiCol_CheckMark]            = ImVec4(0.64f, 0.58f, 0.95f, 1.00f);
    colors[ImGuiCol_SliderGrab]           = ImVec4(0.53f, 0.45f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.64f, 0.58f, 0.95f, 1.00f);

    // Text & selections
    colors[ImGuiCol_Text]                 = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.35f, 0.29f, 0.68f, 0.35f);
}

static Cadmodel g_cad_model;
static std::string g_stl_path;
static int g_num_facets = 0;
static bool g_show_error = false;
static std::string g_error_message;

static int g_test_status = 0; // 0 = Not started, 1 = Running, 2 = Success
static float g_test_timer = 0.0f;
static bool selected[12];

static void render_widgets() {
    // 1. Check for simulation timer completion
    if (g_test_status == 1 && ImGui::GetTime() > g_test_timer) {
        g_test_status = 2; // Success
    }

    // 2. Open popups if needed
    if (show_about) {
        ImGui::OpenPopup("About Myapp");
        show_about = false;
    }
    show_about_dialog();

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    // 3. Fullscreen workspace window
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Workspace Dashboard", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    // 4. Main Menu Bar inside fullscreen window
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open STL File...", "Ctrl+O")) {
                IGFD::FileDialogConfig config;
                config.path = "./data";
                IGFD::FileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", "((.*)),.stl", config);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                show_about = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // 5. File Dialog Display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            g_show_error = false;
            g_stl_path = ImGuiFileDialog::Instance()->GetFilePathName();
            try {
                g_cad_model = Cadmodel();
                g_cad_model.open(g_stl_path);
                g_num_facets = g_cad_model.facets().size();
            } catch (const CadmodelError& e) {
                g_show_error = true;
                g_error_message = e.what();
                g_num_facets = 0;
            }
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // 6. Left Sidebar Panel (Width: 320px)
    ImGui::BeginChild("Sidebar", ImVec2(320, 0), true, ImGuiWindowFlags_NoScrollbar);
    
    ImGui::TextColored(ImVec4(0.64f, 0.58f, 0.95f, 1.00f), "CONTROL CENTER");
    ImGui::Separator();
    ImGui::Spacing();

    // Section A: STL Loader
    ImGui::Text("CAD FILE LOADER");
    if (ImGui::Button("Open STL File...", ImVec2(-1, 40))) {
        IGFD::FileDialogConfig config;
        config.path = "./data";
        IGFD::FileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", "((.*)),.stl", config);
    }
    
    if (g_num_facets > 0) {
        std::string filename = g_stl_path.substr(g_stl_path.find_last_of("/\\") + 1);
        ImGui::Text("File: %s", filename.c_str());
        ImGui::Text("Facets: %d", g_num_facets);
    } else {
        ImGui::Text("No STL file loaded");
    }

    if (g_show_error) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Error: %s", g_error_message.c_str());
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Section B: Test Runner Configuration
    ImGui::Text("TEST CONFIGURATION");
    static int build_type = 0;
    ImGui::RadioButton("Debug Build", &build_type, 0); ImGui::SameLine();
    ImGui::RadioButton("Release Build", &build_type, 1);
    ImGui::Spacing();

    if (ImGui::Button("Select All", ImVec2(140, 0))) {
        for (int i = 0; i < 12; i++) selected[i] = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Select None", ImVec2(140, 0))) {
        for (int i = 0; i < 12; i++) selected[i] = false;
    }
    
    ImGui::Spacing();
    if (ImGui::Button("Run Selected Tests", ImVec2(-1, 45))) {
        g_test_status = 1; // Running
        g_test_timer = (float)ImGui::GetTime() + 1.2f; // Simulate for 1.2s
    }

    ImGui::Spacing();
    ImGui::Text("Test Status: ");
    ImGui::SameLine();
    if (g_test_status == 0) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Idle");
    } else if (g_test_status == 1) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Running...");
    } else if (g_test_status == 2) {
        ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Completed (Success)");
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Section C: Global Settings
    ImGui::Text("GLOBAL SETTINGS");
    ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
    ImGui::Checkbox("Show ImGui Demo Window", &show_demo_window);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Performance Info at the bottom of Sidebar
    ImGui::Text("PERFORMANCE INFO");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::EndChild();

    ImGui::SameLine();

    // 7. Right Panel (Remaining space for Tabs)
    ImGui::BeginChild("MainArea", ImVec2(0, 0), false);
    
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("WorkspaceTabBar", tab_bar_flags)) {
        
        // Tab 1: STL Inspector
        if (ImGui::BeginTabItem("STL Geometry Inspector")) {
            if (g_num_facets == 0) {
                ImGui::Spacing();
                ImGui::Text("No STL model loaded. Please click 'Open STL File...' on the sidebar to load and inspect geometry data.");
            } else {
                ImGui::Text("STL Model: %s", g_stl_path.c_str());
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::Text("Facet Database (showing first 200 elements):");
                if (ImGui::BeginTable("StlTable", 5, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY, ImVec2(0, 480))) {
                    ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                    ImGui::TableSetupColumn("Normal Vector (X, Y, Z)");
                    ImGui::TableSetupColumn("Vertex 1 (X, Y, Z)");
                    ImGui::TableSetupColumn("Vertex 2 (X, Y, Z)");
                    ImGui::TableSetupColumn("Vertex 3 (X, Y, Z)");
                    ImGui::TableHeadersRow();

                    const auto& facets = g_cad_model.facets();
                    int display_count = facets.size() < 200 ? facets.size() : 200;
                    for (int i = 0; i < display_count; i++) {
                        const auto& facet = facets[i];
                        ImGui::TableNextRow();
                        
                        // Index
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("#%d", i + 1);
                        
                        // Normal
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.4f, %.4f, %.4f", facet.normal().x(), facet.normal().y(), facet.normal().z());
                        
                        // Vertices
                        if (facet.points().size() == 3) {
                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("%.4f, %.4f, %.4f", facet.points()[0].x(), facet.points()[0].y(), facet.points()[0].z());
                            
                            ImGui::TableSetColumnIndex(3);
                            ImGui::Text("%.4f, %.4f, %.4f", facet.points()[1].x(), facet.points()[1].y(), facet.points()[1].z());
                            
                            ImGui::TableSetColumnIndex(4);
                            ImGui::Text("%.4f, %.4f, %.4f", facet.points()[2].x(), facet.points()[2].y(), facet.points()[2].z());
                        } else {
                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("Invalid geometry data (missing vertices)");
                        }
                    }
                    ImGui::EndTable();
                }
            }
            ImGui::EndTabItem();
        }

        // Tab 2: Test Case Selection
        if (ImGui::BeginTabItem("Test Runner Dashboard")) {
            ImGui::Text("Select the test cases you wish to run:");
            ImGui::Spacing();
            
            // Checkboxes table
            if (ImGui::BeginTable("TestCaseGrid", 3, ImGuiTableFlags_Borders)) {
                int case_idx = 0;
                for (int row = 0; row < 4; row++) {
                    ImGui::TableNextRow();
                    for (int col = 0; col < 3; col++) {
                        ImGui::TableSetColumnIndex(col);
                        char name[50];
                        sprintf(name, "Test Case %d", case_idx);
                        ImGui::Checkbox(name, &selected[case_idx]);
                        case_idx++;
                    }
                }
                ImGui::EndTable();
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            ImGui::Text("Test Console / Output Log:");
            ImGui::BeginChild("ConsoleLog", ImVec2(0, 200), true);
            if (g_test_status == 0) {
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "System idle. Awaiting command to run tests...");
            } else if (g_test_status == 1) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[RUN] Initializing Doctest suite...");
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[RUN] Running selected test cases...");
            } else if (g_test_status == 2) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[doctest] doctest version is \"2.4.11\"");
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[doctest] run with \"--help\" for options");
                ImGui::Text("===============================================================================");
                int selected_count = 0;
                for (int i = 0; i < 12; i++) if (selected[i]) selected_count++;
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[doctest] test cases: %d | %d passed | 0 failed | 0 skipped", selected_count, selected_count);
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[doctest] Status: SUCCESS!");
            }
            ImGui::EndChild();
            
            ImGui::EndTabItem();
        }

        // Tab 3: Performance Plots
        if (ImGui::BeginTabItem("System Plots")) {
            static bool animate = true;
            ImGui::Checkbox("Animate Cosine Wave", &animate);
            
            ImGui::Text("Memory Allocation (Simulated)");
            static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
            ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));
            ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr),
                    0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));

            static float values[90] = {};
            static int values_offset = 0;
            static double refresh_time = 0.0;

            if (!animate || refresh_time == 0.0)
                refresh_time = ImGui::GetTime();

            while (refresh_time < ImGui::GetTime()) {
                static float phase = 0.0f;
                values[values_offset] = cosf(phase);
                values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
                phase += 0.10f * values_offset;
                refresh_time += 1.0f / 60.0f;
            }

            {
                float average = 0.0f;
                for (int n = 0; n < IM_ARRAYSIZE(values); n++)
                    average += values[n];
                average /= (float)IM_ARRAYSIZE(values);
                char overlay[32];
                sprintf(overlay, "avg %f", average);
                ImGui::PlotLines("Cosine Waves", values, IM_ARRAYSIZE(values),
                        values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 100.0f));
            }
            
            ImGui::EndTabItem();
        }
        
        // Tab 4: ImPlot Demo
        if (ImGui::BeginTabItem("ImPlot Demo Window")) {
            ImPlot::ShowDemoWindow();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::EndChild();

    ImGui::End();
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

/* #if defined(__APPLE__) */
/*     // GL 3.2 + GLSL 150 */
/*     const char* glsl_version = "#version 150"; */
/*     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); */
/*     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); */
/*     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only */
/*     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac */
/* #else */
/*     const char* glsl_version = "#version 130"; */
/*     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); */
/*     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); */
/* #endif */
    GLFWwindow* window = glfwCreateWindow(1024, 720,
                                          "Test Dear ImGui", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    apply_premium_theme();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
    
    setup_fonts();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
       
        render_widgets();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                clear_color.y * clear_color.w,
                clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
