#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "ImGuiFileDialog.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "cadmodel.h"
#include <iostream>

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
    io.Fonts->AddFontDefault();
    ImFont *font = io.Fonts->Fonts.back();
    font->Scale = 1.5f;
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
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static bool show_demo_window = false;
static bool show_test_window = true;

static void show_main_menu_bar() {
    show_about = false;
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                show_about = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

static void show_about_dialog() {
    static bool open = true;
    if (show_about) {
        ImGui::OpenPopup("About Myapp");
        open = true;
    }

    if (ImGui::BeginPopupModal("About Myapp", &open,
                ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("imguiplay");
        ImGui::Separator();

        ImGui::Text("Version: 1.0.0");
        ImGui::Text("Author: skyera");

        ImGui::Spacing();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("License");
        ImGui::Text("This application is licensed under the MIT License.");

        ImGui::Spacing();
        if (ImGui::Button("Visit GitHub")) {
#ifdef _WIN32
            system("start https://github.com/skeyera/imguiplay");
#elif __APPLE__
            system("open https://github.com/skyera/imguiplay");
#else
            system("xdg-open https://github.com/skyera/imguiplay");
#endif
        }

        ImGui::Spacing();
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

static void render_window1() {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("My Test");

    ImGui::Text("This is some useful text.");              
    ImGui::Checkbox("Demo Window", &show_demo_window);    

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);         
    ImGui::ColorEdit3("clear color", (float*)&clear_color); 

    if (ImGui::Button("Button"))                           
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate);
    ImGui::End();
}

static bool selected[12];
void render_test_case_table() {
    ImGui::Text("Test Cases");
    if (ImGui::BeginTable("Test Cases", 3)) {
        int count = 0;
        for (int row = 0; row < 4; row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 3; column++)
            {
                ImGui::TableSetColumnIndex(column);
                char name[100];
                sprintf(name, "Test Case %d", count);
                ImGui::Checkbox(name, &selected[count]);
                ++count;
            }
        }
        ImGui::EndTable();
    }
}

static void render_test_window() {
    ImGui::Begin("Test in Action", &show_test_window, ImGuiWindowFlags_MenuBar);                          
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) {

            }
            if (ImGui::MenuItem("Demo")) {
                show_demo_window = true;
            }
            if (ImGui::MenuItem("Close", "Ctrl+w")) {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }


    ImGui::Text("Build Type: ");
    ImGui::SameLine();
    static int e = 0;
    ImGui::RadioButton("Debug", &e, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Release", &e, 1);
    
    render_test_case_table();

    if (ImGui::Button("Select All")) {
        for (int i = 0; i < 12; ++i)
            selected[i] = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Select None")) {
        for (int i = 0; i < 12; ++i)
            selected[i] = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Run")) {
        printf("Run\n");
    }

    ImGui::Text("Test Status");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Not started");

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Memory Plot"))
        {
            static bool animate = true;
            ImGui::Checkbox("Animate", &animate);

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
                ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values),
                        values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Test Summary"))
        {
            ImGui::Text("This is the Broccoli tab!");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Test XML"))
        {
            ImGui::Text("This is the Cucumber tab!");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Valgrind"))
        {
            ImGui::Text("This is the Cucumber tab!");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Separator();
    ImGui::End();
}

static void render_blackcat_window() {
    static std::string path;
    static int num_facets = 0;
    static std::string error;
    static bool show_error = false;
    ImGui::Begin("Imgui BlackCat");
    ImGui::Text("Hello Xiaohei!");
    if (ImGui::Button("Open")) {
        IGFD::FileDialogConfig config;
        config.path = "./data";
        IGFD::FileDialog::Instance()->OpenDialog("ChooseFileDlgKey",
                "Choose File", "((.*)),.stl", config);
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            show_error = false;
            path = ImGuiFileDialog::Instance()->GetFilePathName();
            Cadmodel model;

            try {
                model.open(path);
                num_facets = model.facets().size();
            } catch (const CadmodelError& e) {
                printf("Error: %s\n", e.what());
                //show_error_dialog(e.what());
                show_error = true;
                error = e.what();
            }
        }
        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::SameLine();
    ImGui::Text("Path: %s", path.c_str());
    ImGui::Text("# facets: %d", num_facets);

    if (show_error) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", error.c_str());
    }

    ImGui::End();
}

static void render_widgets() {
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    show_main_menu_bar();
    show_about_dialog();

    render_window1();
    render_test_window();
    render_blackcat_window();
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
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
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
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
