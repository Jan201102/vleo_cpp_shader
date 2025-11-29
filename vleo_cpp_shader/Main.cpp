#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLUtils.h"

//math includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//geometry data
#include "geometries/tetraeder.h"

//custom abstractions
#include "VertexBuffer.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Renderer.h"

int main(void)
{
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
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle Renderer", nullptr, nullptr);
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
    unsigned int NUM_PIXEL = 800;
    // add framebuffer with texture to store triangle ids
    //TODO: atkuell nur byte für ids verwendet -> nur 256 triangles
    unsigned int framebuffer, depthBuffer, IDtexture;

    //FrameBuffer FB(IDtexture, NUM_PIXEL, NUM_PIXEL);
    //FB.UnBind();
    
    //framebuffer erstellen
    GLCall(glGenFramebuffers(1, &framebuffer));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));

    //texture erstellen um ids aufzunehmen
    GLCall(glGenTextures(1, &IDtexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, IDtexture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, NUM_PIXEL, NUM_PIXEL, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr));


    //texture an framebuffer anhängen
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, IDtexture, 0));

    //depthbuffer hinzufügen
    GLCall(glGenRenderbuffers(1, &depthBuffer));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, NUM_PIXEL, NUM_PIXEL));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));

    // 5. Framebuffer-Vollständigkeit prüfen
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete! Status: " << status << std::endl;
        return -1;
    }

    //Zurück zum Standard-Framebuffer
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // histogrambuffer für computeshader um pixel zu zählen
    const int MAX_TRIANGLES = 65536 - 1;
    unsigned int histogramBuffer;

    GLCall(glGenBuffers(1, &histogramBuffer));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_TRIANGLES * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));


    //projection matrices
    glm::vec3 windDirection(0.0f, 0.0f, 1.0f);
    float cameraDistance = std::sqrt(3);
    glm::vec3 camera_position = -windDirection * cameraDistance / glm::length(windDirection);

    glm::mat4 orthoProj = glm::ortho(-cameraDistance, cameraDistance, -cameraDistance, cameraDistance, 0.0f, 2*cameraDistance);
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
    VertexBuffer vb(vertices, sizeof(vertices));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));
    vb.Unbind();

    VertexBuffer vbID(triangleIDs, sizeof(triangleIDs));
    GLCall(glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(unsigned int), (void*)0));
    GLCall(glEnableVertexAttribArray(1));
    vbID.Unbind();
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyway so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    GLCall(glBindVertexArray(0));
    
    // Create shader program
    Shader shader("res/shaders/Basic.shader");
    shader.Unbind();
    ComputeShader computeShader("res/shaders/Compute.shader");
    computeShader.Unbind();
    
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //=======================
        // Render zum framebuffer
        //=======================

        // PHASE 1: Zu ID-Framebuffer rendern
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
        /*FB.Bind();*/

        // Integer-Clear für ID-Framebuffer (Hintergrund = 0)
        GLuint clearColor[4] = { 0, 0, 0, 0 };
        GLCall(glClearBufferuiv(GL_COLOR, 0, clearColor));
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));

        // Triangle-IDs rendern
        shader.Bind();
        GLCall(glBindVertexArray(VAO));
       /* GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_MVP"), 1, GL_FALSE, &u_MVP[0][0]));*/
        shader.setUniformMat4f("u_MVP", u_MVP);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        // Histogram-Buffer leeren
        GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer));
        GLuint* histogramData = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        if (histogramData) {
            memset(histogramData, 0, MAX_TRIANGLES * sizeof(GLuint));
            GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
        }

        // ID-Texture für Compute-Shader binden (binding = 0)
        GLCall(glBindImageTexture(0, IDtexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16UI));

        // Histogram-Buffer für Compute-Shader binden (binding = 1)
        GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, histogramBuffer));

        /*GLCall(glUseProgram(computeShaderProgram));*/
        computeShader.Bind();

        // Compute-Shader dispatchen (16x16 Work Groups)
        GLCall(glDispatchCompute((NUM_PIXEL + 15) / 16, (NUM_PIXEL + 15) / 16, 1));

        // Warten bis Compute-Shader fertig ist
        GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        // Histogram-Ergebnisse auslesen
        GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer));
        histogramData = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        
		bool visibleIDsFound[numTriangleIDs] = { false };
        if (histogramData) {
            std::cout << "Triangle Histogram:" << std::endl;
            for (int i = 1; i < 10; i++) { // Nur erste 10 Triangle-IDs anzeigen
                if (histogramData[i] > 0) {
					visibleIDsFound[i] = true;
                    std::cout << "Triangle ID " << i << ": " << histogramData[i] << " pixels" << std::endl;
                }
            }
            GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
        }

        //======================
        // Render auf den screen
        //======================
        
        // Render
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw triangle
        shader.Bind();
        GLCall(glBindVertexArray(VAO));
        shader.setUniformMat4f("u_MVP", u_MVP);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    GLCall(glDeleteFramebuffers(1, &framebuffer));
    GLCall(glDeleteTextures(1, &IDtexture));
    GLCall(glDeleteRenderbuffers(1, &depthBuffer));
    GLCall(glDeleteBuffers(1, &histogramBuffer));
    GLCall(glDeleteVertexArrays(1, &VAO));
    
    glfwTerminate();
    return 0;
}