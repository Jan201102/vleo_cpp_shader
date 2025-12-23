#pragma once
#include "BinaryShader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//math includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//custom abstractions
#include "VertexBuffer.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Renderer.h"
#include "FrameBuffer.h"

// embedded shader headers
#include "res/shaders/ID_shader.h"
#include "res/shaders/Compute_shader.h"
#include "res/shaders/Color_shader.h"


int BinaryRenderer(float vertices[], size_t lenVertices, unsigned int triangleIDs[], size_t lenTriangleIDs, bool shaded[], size_t lenShaded,
    float windX, float windY, float windZ) {

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Fenster unsichtbar machen

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Triangle Renderer", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    {
        // framebuffer um ids zu z�hlen
        unsigned int NUM_PIXEL = 800;
        unsigned int IDtexture;
        GLCall(glGenTextures(1, &IDtexture));
        GLCall(glBindTexture(GL_TEXTURE_2D, IDtexture));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, NUM_PIXEL, NUM_PIXEL, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr));

        FrameBuffer FB(IDtexture, NUM_PIXEL, NUM_PIXEL);
        FB.UnBind();


        // histogrambuffer f�r computeshader um pixel zu z�hlen
        const int MAX_TRIANGLES = 65536 - 1;
        unsigned int histogramBuffer;

        GLCall(glGenBuffers(1, &histogramBuffer));
        GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer));
        GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_TRIANGLES * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));
        GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));


        //projection matrices
        glm::vec3 GLMwindDirection(windX, windY, windZ);
        float cameraDistance = std::sqrt(3);
        glm::vec3 camera_position = -GLMwindDirection * cameraDistance / glm::length(GLMwindDirection);

        glm::mat4 orthoProj = glm::ortho(-cameraDistance, cameraDistance, -cameraDistance, cameraDistance, 0.0f, 2 * cameraDistance);
        glm::mat4 view = glm::lookAt(
            camera_position, // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f), // Look at point
            glm::vec3(0.0f, 1.0f, 0.0f)  //TOdO problem with upvector || to winddirection?
        );
        //glm::mat4 view = glm::mat4(1.0f); // Identity matrix for view
        glm::mat4 model = glm::mat4(1.0f); // Identity matrix for model
        glm::mat4 u_MVP = orthoProj * view * model;

        // Create and configure vertex buffer and vertex array objects
        unsigned int VAO;
        GLCall(glGenVertexArrays(1, &VAO));

        // Bind VAO first, then bind and set vertex buffer(s), and then configure vertex attributes
        GLCall(glBindVertexArray(VAO));
        VertexBuffer vb(vertices, sizeof(float) * lenVertices);
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
        GLCall(glEnableVertexAttribArray(0));
        vb.Unbind();

        VertexBuffer vbID(triangleIDs, sizeof(unsigned int) * lenTriangleIDs);
        GLCall(glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(unsigned int), (void*)0));
        GLCall(glEnableVertexAttribArray(1));
        vbID.Unbind();
        GLCall(glBindVertexArray(0));


        // Create shader program from embedded sources
        Shader shader(ID_vertex_shader, ID_fragment_shader, true);
        shader.Unbind();
        ComputeShader computeShader(Compute_shader, true);
        computeShader.Unbind();
        Shader colorShader(Color_vertex_shader, Color_fragment_shader, true);
        colorShader.Unbind();


        // Render loop
            // Input handling
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //=======================
        // Render zum framebuffer
        //=======================

        // PHASE 1: Zu ID-Framebuffer rendern
        FB.Bind();
        FB.Clear();

        // Triangle-IDs rendern
        shader.Bind();
        GLCall(glBindVertexArray(VAO));
        shader.setUniformMat4f("u_MVP", u_MVP);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        // Histogram-Buffer leeren
        GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer));
        GLuint* histogramData = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        if (histogramData) {
            memset(histogramData, 0, MAX_TRIANGLES * sizeof(GLuint));
            GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
        }

        // ID-Texture f�r Compute-Shader binden (binding = 0)
        GLCall(glBindImageTexture(0, IDtexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16UI));

        // Histogram-Buffer f�r Compute-Shader binden (binding = 1)
        GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, histogramBuffer));

        computeShader.Bind();

        // Compute-Shader dispatchen (16x16 Work Groups)
        GLCall(glDispatchCompute((NUM_PIXEL + 15) / 16, (NUM_PIXEL + 15) / 16, 1));

        // Warten bis Compute-Shader fertig ist
        GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        // Histogram-Ergebnisse auslesen
        GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer));
        histogramData = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

        if (histogramData) {
            //std::cout << "Triangle Histogram:" << std::endl;
            for (int i = 0; i < lenTriangleIDs/3; i++) { // Nur erste 10 Triangle-IDs anzeigen
                if(histogramData[i+1] > 0){
                    shaded[i] = true;
                    std::cout << "Triangle ID " << i+1 << ": " << histogramData[i+1] << " pixels" << std::endl;
                }
            }
            GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
        }

        //======================
        // Render auf den screen
        //======================

        // Render
        //GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// Draw triangle
        //colorShader.Bind();
        //GLCall(glBindVertexArray(VAO));
        //shader.setUniformMat4f("u_MVP", u_MVP);
        //glDrawArrays(GL_TRIANGLES, 0, 12);

        //// Swap buffers and poll IO events
        //glfwSwapBuffers(window);
        //glfwPollEvents();
        // Clean up
        GLCall(glDeleteTextures(1, &IDtexture));
        GLCall(glDeleteBuffers(1, &histogramBuffer));
        GLCall(glDeleteVertexArrays(1, &VAO));
    }

    
    glfwTerminate();
    return 0;
}