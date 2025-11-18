#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "./header/Object.h"
#include "./header/stb_image.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int createShader(const string &filename, const string &type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int modelVAO(Object &model);
unsigned int loadTexture(const char *tFileName);
void init();

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// Shader
unsigned int vertexShader, fragmentShader, shaderProgram;

// Texture
unsigned int groundTexture, fishTexture, columnTexture;

// VAO, VBO
unsigned int groundVAO, fishVAO, columnVAO;

// Objects to display
Object *groundObject, *fishObject, *columnObject;

// Constants you may need
const int rotateColumnSpeed = 10;
const int revolveFishSpeed = 20;
const int rotateFishSpeed = 180;
const float fishColumnDist = 3;
const float heightSpeed = 0.25;
const int squeezeSpeed = 90;

// Variables you can use
float rotateColumnDegree = 0;
float revolveFishDegree = 0;
float rotateFishDegree = 0; 
float fishColumnOffset = 0;
int heightDir = -1; 
float fishHeight = 0;

bool useSelfRotation = false;
bool useSqueeze = false;
float squeezeFactor = 0;
bool useBreathing = false; 
float intensity = 1.0;
glm::vec3 breathingColor = glm::vec3(1.0f, 1.0f, 1.0f);

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    /* TODO#0: Change window title to "HW2 - [your student id]"
     *        Ex. HW2-112550000
     */

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW2-your student id", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO#1: Finish function createShader
    // TODO#2: Finish function createProgram
    // TODO#3: Finish function modelVAO
    // TODO#4: Finish function loadTexture
    // You can find the above functions right below the main function

    // Initialize Object, Shader, Texture, VAO, VBO
    init();

    // Enable depth test, face culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Set viewport
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // render loop variables
    double dt;
    double lastTime = glfwGetTime();
    double currentTime;

    /* TODO#5: Data connection - Retrieve uniform variable locations
     *    1. Retrieve locations for model, view, and projection matrices.
     *    2. Retrieve locations for squeezeFactor, breathingColor, intensity, and other parameters.
     * Hint:
     *    glGetUniformLocation
     */

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // render color of water
        glClearColor(0.15, 0.50, 0.65, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 8.5f, 13.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        glm::mat4 groundModel(1.0f), fishModel(1.0f), columnModel(1.0f);

        /* TODO#6-1: Render Ground
         *    1. Set up ground model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, breathingColor, intensity, or other parameters to the program.
         *    4. Apply the texture, and render the ground.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */

        /* TODO#6-2: Render Column
         *    1. Set up column model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, breathingColor, intensity, or other parameters to the program.
         *    4. Apply the texture, and render the column.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */

        /* TODO#6-3: Render Fish
         *    1. Set up fish model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, breathingColor, intensity, or other parameters to the program.
         *    4. Apply the texture, and render the fish.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */


        // Status update
        currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;

        /* TODO#7: Update "revolveFishDegree", "rotateColumnDegree", "rotateFishDegree", 
         *          "fishHeight", "heightDir", 
         *          "squeezeFactor", "breathingColor", "intensity"
         */

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE.
    // Events with GLFW_PRESS and GLFW_RELEASE actions are emitted for every key press.
    // Most keys will also emit events with GLFW_REPEAT actions while a key is held down.
    // https://www.glfw.org/docs/3.3/input_guide.html

    /* TODO#8: Key callback 
    *    1. Press 'r' to rotate the fish.
    *    2. Press 's' to squeeze the fish.
    *    3. Press 'b' to make the breathing light.
    * Hint:
    *      GLFW_KEY_R, GLFW_KEY_S, GLFW_KEY_B
    *      GLFW_PRESS, GLFW_REPEAT
    */    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

/* TODO#1: createShader
 * input:
 *      filename: shader file name
 *      type: shader type, "vert" means vertex shader, "frag" means fragment shader
 * output: shader object
 * Hint:
 *      glCreateShader, glShaderSource, glCompileShader
 */
unsigned int createShader(const string &filename, const string &type) {
}

/* TODO#2: createProgram
 * input:
 *      vertexShader: vertex shader object
 *      fragmentShader: fragment shader object
 * output: shader program
 * Hint:
 *      glCreateProgram, glAttachShader, glLinkProgram, glDetachShader
 */
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
}

/* TODO#3: modelVAO
 * input:
 *      model: Object you want to render
 * output: VAO
 * Hint:
 *      glGenVertexArrays, glBindVertexArray, glGenBuffers, glBindBuffer, glBufferData,
 *      glVertexAttribPointer, glEnableVertexAttribArray,
 */
unsigned int modelVAO(Object &model) {
}

/* TODO#4: loadTexture
 * input:
 *      filename: texture file name
 * output: texture object
 * Hint:
 *      glEnable, glGenTextures, glBindTexture, glTexParameteri, glTexImage2D
 */
unsigned int loadTexture(const string &filename) {
}

void init() {
#if defined(__linux__) || defined(__APPLE__)
    string dirShader = "../../src/shaders/";
    string dirAsset = "../../src/asset/obj/";
    string dirTexture = "../../src/asset/texture/";
#else
    string dirShader = "..\\..\\src\\shaders\\";
    string dirAsset = "..\\..\\src\\asset\\obj\\";
    string dirTexture = "..\\..\\src\\asset\\texture\\";
#endif

    // Object
    fishObject = new Object(dirAsset + "fish.obj");
    columnObject = new Object(dirAsset + "column.obj");
    groundObject = new Object(dirAsset + "cube.obj");

    // Shader
    vertexShader = createShader(dirShader + "vertexShader.vert", "vert");
    fragmentShader = createShader(dirShader + "fragmentShader.frag", "frag");
    shaderProgram = createProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    // Texture
    fishTexture = loadTexture(dirTexture + "fish.jpg");
    columnTexture = loadTexture(dirTexture + "column.jpg");
    groundTexture = loadTexture(dirTexture + "ground.jpg");

    // VAO, VBO
    fishVAO = modelVAO(*fishObject);
    columnVAO = modelVAO(*columnObject);
    groundVAO = modelVAO(*groundObject);
}

