#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(800, 720,
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
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_demo_window = false;
    bool show_another_window = false;
    bool show_test_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("My Test");

            ImGui::Text("This is some useful text.");              
            ImGui::Checkbox("Demo Window", &show_demo_window);    
            ImGui::Checkbox("Another Window", &show_another_window);

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

        {
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
            
            ImGui::Text("Test Cases");
            static bool selected[12];
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

        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                clear_color.y * clear_color.w,
                clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
