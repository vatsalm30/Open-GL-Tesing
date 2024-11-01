#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

	std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
             -50.0f, -50.0f, 0, 0,// 0
             50.0f, -50.0f, 1, 0,// 1
             50.0f, 50.0f, 1, 1,// 2
             -50.0f, 50.0f, 0, 1,// 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
		va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("uColor", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/grass.png");
        texture.Bind();
		shader.SetUniform1i("uTexture", 0);


		va.Unbind();
		vb.Unbind();
		ib.Unbind();
        shader.Unbind();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        float r = 0.0f;
        float increment = 0.05f;

		Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);

        ImGui::StyleColorsDark();

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
			renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            shader.Bind();
            //{
            //    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            //    glm::mat4 mvp = proj * view * model;
            //    shader.SetUniformMat4f("u_MVP", mvp);
            //    renderer.Draw(va, ib, shader);
            //}
			for (int i = 0; i < 100; i++)
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA + glm::vec3(100.0f * (i / 10), 100.0f * (i % 10), 0.0f));
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //{
            //    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            //    glm::mat4 mvp = proj * view * model;
            //    shader.SetUniformMat4f("u_MVP", mvp);
            //    renderer.Draw(va, ib, shader);
            //}

            r += increment;
            {
                ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            if (r > 1.0f)
            {
                increment *= -1;
            }
            else if (r < 0.0f)
            {
                increment *= -1;
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }


    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
