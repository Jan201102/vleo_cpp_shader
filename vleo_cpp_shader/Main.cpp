#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLUtils.h"

//math includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
    
    // Triangle vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // Bottom left
         0.5f, -0.5f, 0.0f,  // Bottom right
         0.0f,  0.5f, 0.0f   // Top
    };

    //projection matrices
    glm::vec3 windDirection(-1.0f, 0.0f, 0.0f);
    float cameraDistance = std::sqrt(3);
    glm::vec3 camera_position = -windDirection * cameraDistance / glm::length(windDirection);

    glm::mat4 orthoProj = glm::ortho(-cameraDistance,cameraDistance, -cameraDistance, cameraDistance, -cameraDistance, cameraDistance);
    glm::mat4 view = glm::lookAt(
        camera_position, // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // Look at point
        glm::vec3(0.0f, 0.0f, -1.0f)  // Up vector -z is up in aerospace coordinate conventions
    );
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix for model
    glm::mat4 u_MVP = orthoProj * view * model;
    
    // Create and configure vertex buffer and vertex array objects
    unsigned int VBO, VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));
    
    // Bind VAO first, then bind and set vertex buffer(s), and then configure vertex attributes
    GLCall(glBindVertexArray(VAO));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));
    
    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyway so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    GLCall(glBindVertexArray(0));
    
    // Create shader program
    ShaderProgramSource shaderPrograms = ParseShader("res/shaders/Basic.shader");
    unsigned int shaderProgram = CreateShader(shaderPrograms.VertexSource,shaderPrograms.FragmentSource,shaderPrograms.ComputeSource);
    
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw triangle
        GLCall(glUseProgram(shaderProgram));
        GLCall(glBindVertexArray(VAO));
        GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_MVP"), 1, GL_FALSE, &u_MVP[0][0]));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    GLCall(glDeleteVertexArrays(1, &VAO));
    GLCall(glDeleteBuffers(1, &VBO));
    GLCall(glDeleteProgram(shaderProgram));
    
    glfwTerminate();
    return 0;
}