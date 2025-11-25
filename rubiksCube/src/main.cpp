#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.h"
#include "camera.h"
#include "form.h"
#include "rubiks.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // MERCI MR.SEAN BARRET :')

#include <iostream>
#include <vector>
#include <array>

//random
#include <cstdlib>
#include <ctime>

RubiksCube rubiksCube;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 500;
const unsigned int SCR_HEIGHT = 500;


//camera
Camera camera(glm::vec3(3.0f, 1.0f, 3.0f));
bool firstMouse = true;
float lastX =  800.0f;
float lastY =  600.0;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void getColorRubiks(char c, float & r, float & g,float & b){
    if(c=='w'){
        r=1.0f;
        g=1.0f;
        b=1.0f;
    }else if(c=='v'){
        r=0.0f;
        g=1.0f;
        b=0.0f;
    }else if(c=='b'){
        r=0.0f;
        g=0.0f;
        b=1.0f;
    }else if(c=='r'){
        r=1.0f;
        g=0.0f;
        b=0.0f;
    }else if(c=='o'){
        r=1.0f;
        g=0.5f;
        b=0.0f;
    }else if(c=='j'){
        r=1.0f;
        g=1.0f;
        b=0.0f;
    }else{
        r=1.0f;
        g=1.0f;
        b=0.0f;
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); //enable z-buffer

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("../src/shader.vs", "../src/shader.fs");

    Cube cube;
    cube.make();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //gestion du temps
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear z-buffer

        // bind textures on corresponding texture units
        //glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
  
        // activate shader
        ourShader.use();
      
        /// camera/view transformation
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.02f, 100.0f);
        ourShader.setMat4("projection", projection);
        
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render container
        glBindVertexArray(cube.VAO);
        
        for (int i = 0; i < 6; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        

        switch (i) {
            case 0: 
                model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); 
                break;
            case 1:
                model = glm::translate(model, glm::vec3(3.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            case 2: 
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -1.0f));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
                break;
            case 3:
                model = glm::translate(model, glm::vec3(0.0f, 2.0f, -2.0f));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                break;
            case 4:
                model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            case 5:
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                break;
        }

        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                glm::mat4 faceModel = model;
                faceModel = glm::translate(faceModel, glm::vec3(1.0f * j, -1.0f * k, 0.0f));

                float r, g, b;
                getColorRubiks(rubiksCube.cube[i].data[j][k], r, g, b);
                ourShader.set4f("color", r, g, b, 1.0f);
                ourShader.setMat4("model", faceModel);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cube.glDelete();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = static_cast<float>(2.5 * deltaTime); // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(SPRINT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        camera.ProcessKeyboard(WALK, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(JUMP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        std::cout<<"Entrez mouvmenent : ";
        std::string mouvement;
        std::getline(std::cin, mouvement);
        rubiksCube.parse(mouvement);
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}