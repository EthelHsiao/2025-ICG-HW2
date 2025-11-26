#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
float intensity = 1.0f;
glm::vec3 breathingColor = glm::vec3(1.0f, 1.0f, 1.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW2-112550179", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    double dt;
    double lastTime = glfwGetTime();
    double currentTime;

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint squeezeFactorLoc = glGetUniformLocation(shaderProgram, "squeezeFactor");
    GLint breathingColorLoc = glGetUniformLocation(shaderProgram, "breathingColor");
    GLint intensityLoc = glGetUniformLocation(shaderProgram, "intensity");
    GLint textureLoc = glGetUniformLocation(shaderProgram, "ourTexture");

    while (!glfwWindowShouldClose(window)) {
        // 防止窗口最小化時除以零
        if (SCR_WIDTH == 0 || SCR_HEIGHT == 0) {
            glfwPollEvents();
            continue;
        }

        glClearColor(0.15f, 0.50f, 0.65f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 8.5f, 13.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        glm::mat4 groundModel(1.0f), fishModel(1.0f), columnModel(1.0f);

        // Ground
        groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(0.0f, -5.0f, 8.0f));
        groundModel = glm::scale(groundModel, glm::vec3(35.0f, 1.0f, 25.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(groundModel));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(squeezeFactorLoc, 0.0f);
        glUniform3fv(breathingColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform1f(intensityLoc, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(textureLoc, 0);

        glBindVertexArray(groundVAO);
        glDrawArrays(GL_TRIANGLES, 0, groundObject->positions.size() / 3);
        glBindVertexArray(0);

        // Column
        columnModel = glm::mat4(1.0f);
        columnModel = glm::translate(columnModel, glm::vec3(0.0f, -4.0f, 0.0f));
        columnModel = glm::rotate(columnModel, glm::radians(rotateColumnDegree), glm::vec3(0.0f, 1.0f, 0.0f));
        columnModel = glm::rotate(columnModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        columnModel = glm::scale(columnModel, glm::vec3(0.05f, 0.05f, 0.05f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(columnModel));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(squeezeFactorLoc, 0.0f);
        glUniform3fv(breathingColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform1f(intensityLoc, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, columnTexture);
        glUniform1i(textureLoc, 0);

        glBindVertexArray(columnVAO);
        glDrawArrays(GL_TRIANGLES, 0, columnObject->positions.size() / 3);
        glBindVertexArray(0);

        // Fish
        fishModel = glm::mat4(1.0f);
        float fishX = fishColumnDist * sin(glm::radians(revolveFishDegree));
        float fishZ = fishColumnDist * cos(glm::radians(revolveFishDegree));

        fishModel = glm::translate(fishModel, glm::vec3(fishX, fishHeight, fishZ));
        fishModel = glm::rotate(fishModel, glm::radians(-revolveFishDegree), glm::vec3(0.0f, 1.0f, 0.0f));
        fishModel = glm::rotate(fishModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        if (useSelfRotation) {
            fishModel = glm::rotate(fishModel, glm::radians(-rotateFishDegree), glm::vec3(-1.0f, 0.0f, 0.0f));
        }

        fishModel = glm::scale(fishModel, glm::vec3(0.05f, 0.05f, 0.05f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(fishModel));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        float currentSqueezeFactor = useSqueeze ? squeezeFactor : 0.0f;
        glUniform1f(squeezeFactorLoc, currentSqueezeFactor);

        if (useBreathing) {
            glUniform3fv(breathingColorLoc, 1, glm::value_ptr(breathingColor));
            glUniform1f(intensityLoc, intensity);
        } else {
            glUniform3fv(breathingColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
            glUniform1f(intensityLoc, 1.0f);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fishTexture);
        glUniform1i(textureLoc, 0);

        glBindVertexArray(fishVAO);
        glDrawArrays(GL_TRIANGLES, 0, fishObject->positions.size() / 3);
        glBindVertexArray(0);

        // Update
        currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;

        rotateColumnDegree += rotateColumnSpeed * dt;
        if (rotateColumnDegree >= 360.0f) rotateColumnDegree -= 360.0f;

        revolveFishDegree -= revolveFishSpeed * dt;
        if (revolveFishDegree <= -360.0f) revolveFishDegree += 360.0f;

        fishHeight += heightDir * heightSpeed * dt;
        if (fishHeight > 2.0f) { fishHeight = 2.0f; heightDir = -1; }
        else if (fishHeight < -2.0f) { fishHeight = -2.0f; heightDir = 1; }

        if (useSelfRotation) {
            rotateFishDegree += rotateFishSpeed * dt;
            if (rotateFishDegree >= 360.0f) { rotateFishDegree = 0.0f; useSelfRotation = false; }
        }

        if (useSqueeze) {
            squeezeFactor += glm::radians((float)squeezeSpeed) * dt;
        }

        if (useBreathing) {
            breathingColor = glm::vec3(1.0f, 1.0f, 0.0f);
            intensity = (sin(currentTime * 2.0f) + 1.0f) / 2.0f;
            intensity = intensity * 0.8f + 0.2f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if (!useSelfRotation) { useSelfRotation = true; rotateFishDegree = 0.0f; }
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        useSqueeze = !useSqueeze;
        if (!useSqueeze) squeezeFactor = 0.0f;
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        useBreathing = !useBreathing;
        if (!useBreathing) { breathingColor = glm::vec3(1.0f, 1.0f, 1.0f); intensity = 1.0f; }
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

unsigned int createShader(const string &filename, const string &type) {
    std::ifstream shaderFile(filename);
    if (!shaderFile.is_open()) {
        std::cout << "ERROR::SHADER::FILE_NOT_FOUND: " << filename << std::endl;
        return 0;
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    std::string shaderCode = shaderStream.str();
    const char* shaderCodeCStr = shaderCode.c_str();
    
    unsigned int shader;
    if (type == "vert") shader = glCreateShader(GL_VERTEX_SHADER);
    else if (type == "frag") shader = glCreateShader(GL_FRAGMENT_SHADER);
    else { std::cout << "ERROR::SHADER::UNKNOWN_TYPE: " << type << std::endl; return 0; }
    
    glShaderSource(shader, 1, &shaderCodeCStr, NULL);
    glCompileShader(shader);
    
    GLint success; char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) { glGetShaderInfoLog(shader, 512, NULL, infoLog); std::cout << "ERROR::SHADER::COMPILATION_FAILED (" << filename << ")\n" << infoLog << std::endl; }
    return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint success; char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) { glGetProgramInfoLog(program, 512, NULL, infoLog); std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl; }
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    return program;
}

unsigned int modelVAO(Object &model) {
    unsigned int VAO, VBO[3];
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(3, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(float), &model.positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(float), &model.normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, model.texcoords.size() * sizeof(float), &model.texcoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}

unsigned int loadTexture(const string &filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD: " << filename << std::endl;
    }
    stbi_image_free(data);
    return texture;
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

    fishObject = new Object(dirAsset + "fish.obj");
    columnObject = new Object(dirAsset + "column.obj");
    groundObject = new Object(dirAsset + "cube.obj");

    vertexShader = createShader(dirShader + "vertexShader.vert", "vert");
    fragmentShader = createShader(dirShader + "fragmentShader.frag", "frag");
    shaderProgram = createProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    fishTexture = loadTexture(dirTexture + "fish.jpg");
    columnTexture = loadTexture(dirTexture + "column.jpg");
    groundTexture = loadTexture(dirTexture + "ground.jpg");

    fishVAO = modelVAO(*fishObject);
    columnVAO = modelVAO(*columnObject);
    groundVAO = modelVAO(*groundObject);
}