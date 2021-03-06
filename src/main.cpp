#include <vector>
#include <stdio.h>
#include <camera.h>

#include "application.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "program.h"
#include "geometry.h"

using namespace GGE;

Application * app;

// Put this here for biscuits
// Global stuff for testing
Program * p;
Program * texP;
Texture * brickTex;
Mesh * t1, * t2, * t3, * square, * texturedTriangle, * objCube;
Camera * cam;

// Wireframe or fill
GLenum polygonMode = GL_FILL;

void setupScene()
{
    // Set clear color
    // TODO: Move this to somewhere that can be changed when needed
    glClearColor(1.0, 0.0, 0.0, 1.0);

    // Load shaders
    // TODO: This doesn't go here!
    Shader * vs = new Shader("res/shaders/basicShader.vs", GL_VERTEX_SHADER);
    Shader * fs = new Shader("res/shaders/basicShader.fs", GL_FRAGMENT_SHADER);
    std::vector<Shader *> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);
    p = new Program(shaders);

    // Make texture program
    Shader * texvs = new Shader("res/shaders/textureShader.vs", GL_VERTEX_SHADER);
    Shader * texfs = new Shader("res/shaders/textureShader.fs", GL_FRAGMENT_SHADER);
    std::vector<Shader*> texShaders;
    texShaders.push_back(texvs);
    texShaders.push_back(texfs);
    texP = new Program(texShaders);

    // Brick Texture
    brickTex = new Texture("res/textures/texture-brick.png", GL_TEXTURE_2D);

    // Make camera
    cam = new Camera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -0.01), Y_UNIT_VECTOR);

    std::vector<GLfloat> points;
    std::vector<GLfloat> tex;
    points.push_back(-0.5);
    points.push_back(-0.5);
    points.push_back(0);
    points.push_back(0);
    points.push_back(0.5);
    points.push_back(0);
    points.push_back(0.5);
    points.push_back(-0.5);
    points.push_back(0);

    t1 = new Mesh(points, tex, NULL, p);

    // Tryna make a square?
    // Note: This DOESN'T make a square
    std::vector<GLfloat> points2;
    std::vector<GLfloat> tex2;
    // First Mesh
    points2.push_back(-0.5);
    points2.push_back(-0.5);
    points2.push_back(0);
    points2.push_back(0);
    points2.push_back(0.5);
    points2.push_back(0);
    points2.push_back(0.5);
    points2.push_back(-0.5);
    points2.push_back(0);
    // Second triangle
    points2.push_back(-1.0);
    points2.push_back(-1.0);
    points2.push_back(0.0);
    points2.push_back(-1.0);
    points2.push_back(1.0);
    points2.push_back(0.0);
    points2.push_back(0.0);
    points2.push_back(1.0);
    points2.push_back(0.0);

    t2 = new Mesh(points2, tex2, NULL, p);

    // Let's try another
    std::vector<GLfloat> points3;
    std::vector<GLfloat> tex3;
    points3.push_back(-0.5);
    points3.push_back(0.5);
    points3.push_back(0);
    points3.push_back(0);
    points3.push_back(0.5);
    points3.push_back(0);
    points3.push_back(-0.5);
    points3.push_back(-0.5);
    points3.push_back(0);

    t3 = new Mesh(points3, tex3, NULL, p);

    // Test drawing with strips
    std::vector<GLfloat> squarePts;
    std::vector<GLfloat> squareTex;
    squarePts.push_back(-0.5);
    squarePts.push_back(0.5);
    squarePts.push_back(0.0);
    squarePts.push_back(0.5);
    squarePts.push_back(0.5);
    squarePts.push_back(0.0);
    squarePts.push_back(0.5);
    squarePts.push_back(-0.5);
    squarePts.push_back(0.0);
    squarePts.push_back(-0.5);
    squarePts.push_back(-0.5);
    squarePts.push_back(0.0);

    squareTex.push_back(1.0);
    squareTex.push_back(0.0);
    squareTex.push_back(0.0);
    squareTex.push_back(1.0);
    squareTex.push_back(1.0);
    squareTex.push_back(1.0);
    squareTex.push_back(1.0);
    squareTex.push_back(0.0);

    square = new Mesh(squarePts, squareTex, brickTex, texP, GL_STATIC_DRAW, GL_TRIANGLE_FAN);

    // Test textured triangle
    std::vector<GLfloat> ttPoints;
    std::vector<GLfloat> ttTex;

    ttPoints.push_back(-0.5);
    ttPoints.push_back(-0.5);
    ttPoints.push_back(0.0);
    ttPoints.push_back(0.0);
    ttPoints.push_back(0.5);
    ttPoints.push_back(0.0);
    ttPoints.push_back(0.5);
    ttPoints.push_back(-0.5);
    ttPoints.push_back(0.0);

    ttTex.push_back(0.0);
    ttTex.push_back(1.0);
    ttTex.push_back(0.5);
    ttTex.push_back(0.0);
    ttTex.push_back(1.0);
    ttTex.push_back(1.0);

    texturedTriangle = new Mesh(ttPoints, ttTex, brickTex, texP);
    texturedTriangle->setPosition(glm::vec3(0.0, 0.0, -1.0));

    objCube = new Mesh("cube.obj", "res/meshes", p);
    objCube->setPosition(glm::vec3(1.0, 0.0, -1.0));
}

// Callback for handling key events
void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    switch(key)
    {
        // Change Rendering Mode
        case GLFW_KEY_R:
            if(action == GLFW_PRESS)
            {
                polygonMode = polygonMode == GL_FILL ? GL_LINE : GL_FILL;
                glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
            }
            break;
        // Quit
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        // If there's some other key...probably not needed
        default:
            break;
    }
}

void handleInput()
{
    GLFWwindow * window = Application::mainWindow->getGLFWWindow();

    glm::vec3 direction = glm::normalize(cam->getDirection());
    direction.y = 0;
    glm::vec3 right = glm::normalize(cam->getRight());

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->move(0.05f * direction);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->move(-0.05f * direction);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->move(-0.05f * right);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->move(0.05f * right);
}

// Callback for handling mouse movement
void cursorMoveCallback(GLFWwindow * window, double xpos, double ypos)
{
    // Rotate camera
    Camera * cam = Camera::getCurrentCamera();
    cam->yaw(0.05f * -xpos);
    cam->pitch(0.05f * -ypos);

    // Reset mouse position
    glfwSetCursorPos(window, 0, 0);
}

void renderFunction(double timeSinceLastFrame)
{
    // Update camera physics
    cam->updatePhysics(timeSinceLastFrame / CLOCKS_PER_SEC);

    //t1->draw();
    //t2->draw();
    t3->draw();
    t3->setPosition(glm::vec3(1.0, 0.0, -1.0));

    brickTex->bind();
    //square->draw();
    texturedTriangle->draw();
    objCube->draw();

    // Update View Matrix
    Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());

    handleInput();

    //glm::vec3 pos = texturedTriangle->move(glm::vec3(-0.01, 0.01, 0.0));

    //cam->move(glm::vec3(0.001, 0.0, 0.01));
    //cam->pitch(0.1);
}

int main(int argc, char ** argv)
{
    // Start Application
    app = new Application();

    // Add Window
    Window * window = app->addWindow(320, 620, true, "Test", true);

    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Set up scene
    setupScene();

    // TODO: Move callbacks to setting variables for multiple presses
    // Set key callback
    glfwSetKeyCallback(window->getGLFWWindow(), keyCallback);

    // Set callback for mouse movement
    glfwSetCursorPosCallback(window->getGLFWWindow(), cursorMoveCallback);

    // Set window's render function
    window->setRenderFunction(renderFunction);

    // Render Loop
    while(app->mainWindow->renderFrame());

    // Destroy application
    delete app;

    return 0;
}
