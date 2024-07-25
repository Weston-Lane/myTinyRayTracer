
#include <stdio.h>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <numbers>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image\stb_image.h"
#include "stb_image\stb_image_write.h"

#include "GLFW/glfw3.h" 

#include "Vector.h"

constexpr short JWIDTH = 1024;//jpeg width and height
constexpr short JHEIGHT = 768;
constexpr short WWIDTH = 1280;
constexpr short WHEIGHT = 720;
constexpr float fov = std::numbers::pi_v<float> / 2;
struct Sphere
{
    float radius = 0;
    Vector3<float> center;

    Sphere(Vector3<float>& v, float& r) : center(v), radius(r) {}

    bool m_rayIntersect(const Vector3<float>& orig, const Vector3<float> dir,  float& t) const
    {
        Vector3 o = orig - center;
        float a = std::pow(dir.magnitude(), 2);
        float b = 2 * o.dot(dir);
        float c= std::pow(o.magnitude(), 2)-(radius*radius);

        double disc = (b * b) - (4 * a * c);
        if (disc < 0)
            return false;

        t = -(b)-(std::sqrt(disc)/(2*a));
        float t2= t = -(b)+(std::sqrt(disc) / (2 * a));
        if (t < 0)t = t2;
        if (t < 0) return false;
        
        return true;
    }
};

Vector3<float> castRay(const Vector3<float>& orig,const Vector3<float>& dir, const Sphere& s )
{
    float sphereDist = std::numeric_limits<float>::max();
    if (!s.m_rayIntersect(orig, dir, sphereDist))
    {
        return Vector3(.2f, .7f, .8f);
    }
    return Vector3(.4f, .4f, .3f);
}

void render(const Sphere& s)
{
    std::vector<unsigned char> imageData(JWIDTH * JHEIGHT * 3);
       //writing to imageData buffer and writing that to a jpg
    for (size_t j = 0; j < JWIDTH * JHEIGHT; j++)
    {  
       float x = (2 * ((j % JWIDTH + .5)) / (float)JWIDTH - 1) * tan(fov / 2) * (JWIDTH / (float)JHEIGHT);//j%WIDTH gives the pixel in the row and resets every row
       float y = (2 * ((j / JWIDTH) + .5) / (float)JHEIGHT - 1) * tan(fov / 2);//j/WIDTH gives the truncated value of the division resulting in each row of the image
       Vector3<float> dir = Vector3<float>(x, y, -1).normalize();
       Vector3<float> temp = castRay(Vector3<float>(0, 0,0), dir, s);

       imageData[j * 3] = (unsigned char)(255 * (std::max(0.f, std::min(1.f, temp.x))));//purposfully truncates the divisor decimal with integer division
       imageData[j * 3 + 1] = (unsigned char)(255 * (std::max(0.f, std::min(1.f, temp.y))));
       imageData[j * 3 + 2] = (unsigned char)(255 * (std::max(0.f, std::min(1.f, temp.z))));
    }

    stbi_write_jpg("render.jpg", JWIDTH, JHEIGHT, 3, imageData.data(), 100);
}

GLuint loadImage(const char* file)
{
    int w, h, channels;
    unsigned char* image = stbi_load(file, &w, &h, &channels, 0);

    if (!image)
    {
        std::cerr << "FAILED TO LOAD IMAGE: " << file << std::endl;
        return 0;
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);
    return texture;

}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    // Decide GL+GLSL versions
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(WWIDTH, WHEIGHT, "Tiny Ray Tracer", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLuint imageID = loadImage("./render.jpg");
    Vector3<float> center(0, 0, -16);
    float radius = 2;
    Sphere sphere(center, radius);
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        
        //configuration window
        ImGui::Begin("Configuration");
        //this will run the logic to print the pixels to a jpg
        if (ImGui::SliderFloat(":X", &sphere.center.x, -30.f, 30.f))
        {

        }
        if (ImGui::SliderFloat(":Y", &sphere.center.y, -30.f, 30.f))
        {

        }
        if (ImGui::SliderFloat(":Z", &sphere.center.z, -30.f, 30.f))
        {

        }
        if (ImGui::SliderFloat(":Radius", &sphere.radius,1.f, 10.f))
        {

        }
        if (ImGui::Button("Print to JPG"))
        {
            render(sphere);
            imageID = loadImage("./render.jpg");
        }    
        ImGui::End();
        //image window
        ImGui::Begin("Image");
        ImVec2 resize = ImGui::GetWindowSize();
        ImGui::Image((void*)(intptr_t)imageID, ImVec2((JWIDTH * .7) * (resize.x / resize.y), (JHEIGHT * .7) * (resize.x / resize.y)));
        ImGui::End();
        

        //Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}