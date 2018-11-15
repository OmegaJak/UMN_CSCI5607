// Multi-Object, Multi-Texture Example
// Stephen J. Guy, 2018

// This example demonstrates:
// Loading multiple models (a cube and a knot)
// Using multiple textures (wood and brick)
// Instancing (the teapot is drawn in two locatons)
// Continuous keyboard input - arrows (moves knot up/down/left/right continueous on being held)
// Keyboard modifiers - shift (up/down arrows move knot in/out of screen when shift is pressed)
// Single key events - pressing 'c' changes color of a random teapot
// Mixing textures and colors for models
// Phong lighting
// Binding multiple textures to one shader

#include "constants.h"
#include "shader_manager.h"
#include "texture_manager.h"
const char* INSTRUCTIONS =
    "***************\n"
    "This demo shows multiple objects being draw at once along with user interaction.\n"
    "\n"
    "Up/down/left/right - Moves the knot.\n"
    "c - Changes to teapot to a random color.\n"
    "***************\n";

// Mac OS build: g++ multiObjectTest.cpp -x c glad/glad.c -g -F/Library/Frameworks -framework SDL2 -framework OpenGL -o MultiObjTest
// Linux build:  g++ multiObjectTest.cpp -x c glad/glad.c -g -lSDL2 -lSDL2main -lGL -ldl -I/usr/include/SDL2/ -o MultiObjTest

#include "glad.h"  //Include order can matter here
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include <cstdio>

#define GLM_FORCE_RADIANS
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <cstdio>
#include <string>

#include "model_manager.h"

using namespace std;

int screenWidth = 800;
int screenHeight = 600;
float timePassed = 0;

// SJG: Store the object coordinates
// You should have a representation for the state of each object
float objx = 0, objy = 0, objz = 0;
float colR = 1, colG = 1, colB = 1;
float camx = 3, camy = 0, camz = 0;
float lookatx = 0, lookaty = 0, lookatz = 0;

GLuint InitShader(const char* vShaderFileName, const char* fShaderFileName);
bool fullscreen = false;
void Win2PPM(int width, int height);

// srand(time(NULL));
float rand01() {
    return rand() / (float)RAND_MAX;
}

void drawGeometry(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts);

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);  // Initialize Graphics (for OpenGL)

    // Ask SDL to get a recent version of OpenGL (3.2 or greater)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create a window (offsetx, offsety, width, height, flags)
    SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

    // Create a context to draw in
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Load OpenGL extentions with GLAD
    if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        printf("\nOpenGL loaded\n");
        printf("Vendor:   %s\n", glGetString(GL_VENDOR));
        printf("Renderer: %s\n", glGetString(GL_RENDERER));
        printf("Version:  %s\n\n", glGetString(GL_VERSION));
    } else {
        printf("ERROR: Failed to initialize OpenGL context.\n");
        return -1;
    }

    // Here we will load two different model files
    Model* knot = new Model("models/knot.txt");
    Model* teapot = new Model("models/teapot.txt");

    TextureManager::InitTextures();

    // Build a Vertex Array Object (VAO) to store mapping of shader attributes to VBO
    GLuint vao;
    glGenVertexArrays(1, &vao);  // Create a VAO
    glBindVertexArray(vao);      // Bind the above created VAO to the current context

    ModelManager::InitVBO();

    int texturedShader = ShaderManager::InitShader("textured-Vertex.glsl", "textured-Fragment.glsl");

    TextureManager::InitTextures();

    // GLint colAttrib = glGetAttribLocation(phongShader, "inColor");
    // glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    // glEnableVertexAttribArray(colAttrib);

    glBindVertexArray(0);  // Unbind the VAO in case we want to create a new one

    glEnable(GL_DEPTH_TEST);

    printf("%s\n", INSTRUCTIONS);

    // Event Loop (Loop forever processing each event as fast as possible)
    SDL_Event windowEvent;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&windowEvent)) {  // inspect all events in the queue
            if (windowEvent.type == SDL_QUIT) quit = true;
            // List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
            // Scancode refers to a keyboard position, keycode refers to the letter (e.g., EU keyboards)
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) quit = true;  // Exit event loop
            if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) {                  // If "f" is pressed
                fullscreen = !fullscreen;
                SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);  // Toggle fullscreen
            }

            // SJG: Use key input to change the state of the object
            //     We can use the ".mod" flag to see if modifiers such as shift are pressed
            if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP) {  // If "up key" is pressed
                if (windowEvent.key.keysym.mod & KMOD_SHIFT) {
                    objx -= .1;  // Is shift pressed?
                } else if (windowEvent.key.keysym.mod & KMOD_CTRL) {
                    if (windowEvent.key.keysym.mod & KMOD_ALT) {
                        camx -= 0.1;
                        lookatx -= 0.1;
                    } else {
                        camz += .1;
                        lookatz += 0.1;
                    }
                } else {
                    objz += .1;
                }
            }
            if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN) {  // If "down key" is pressed
                if (windowEvent.key.keysym.mod & KMOD_SHIFT) {
                    objx += .1;  // Is shift pressed?
                } else if (windowEvent.key.keysym.mod & KMOD_CTRL) {
                    if (windowEvent.key.keysym.mod & KMOD_ALT) {
                        camx += 0.1;
                        lookatx += 0.1;
                    } else {
                        camz -= .1;
                        lookatz -= 0.1;
                    }
                } else {
                    objz -= .1;
                }
            }
            if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT) {  // If "left key" is pressed
                if (windowEvent.key.keysym.mod & KMOD_CTRL) {
                    camy -= 0.1;
                    lookaty -= 0.1;
                } else {
                    objy -= .1;
                }
            }
            if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT) {  // If "right key" is pressed
                if (windowEvent.key.keysym.mod & KMOD_CTRL) {
                    camy += 0.1;
                    lookaty += 0.1;
                } else {
                    objy += .1;
                }
            }
            if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_c) {  // If "c" is pressed
                colR = rand01();
                colG = rand01();
                colB = rand01();
            }
        }

        // Clear the screen to default color
        glClearColor(.2f, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(texturedShader);

        timePassed = SDL_GetTicks() / 1000.f;

        glm::mat4 view = glm::lookAt(glm::vec3(camx, camy, camz),           // Cam Position
                                     glm::vec3(lookatx, lookaty, lookatz),  // Look at point
                                     glm::vec3(0.0f, 0.0f, 1.0f));          // Up
        glUniformMatrix4fv(ShaderManager::Attributes.view, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 proj = glm::perspective(3.14f / 4, screenWidth / (float)screenHeight, 1.0f, 10.0f);  // FOV, aspect, near, far
        glUniformMatrix4fv(ShaderManager::Attributes.projection, 1, GL_FALSE, glm::value_ptr(proj));

        TextureManager::Update();

        glBindVertexArray(vao);
        drawGeometry(texturedShader, teapot->vbo_vertex_start_index_, teapot->NumVerts(), knot->vbo_vertex_start_index_, knot->NumVerts());

        SDL_GL_SwapWindow(window);  // Double buffering
    }

    // Clean Up
    ShaderManager::Cleanup();
    ModelManager::Cleanup();
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}

void drawGeometry(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts) {
    glm::vec3 colVec(colR, colG, colB);
    glUniform3fv(ShaderManager::Attributes.color, 1, glm::value_ptr(colVec));

    //************
    // Draw model #1 the first time
    // This model is stored in the VBO starting a offset model1_start and with model1_numVerts num of verticies
    //*************

    // Rotate model (matrix) based on how much time has passed
    glm::mat4 model;
    model = glm::rotate(model, timePassed * 3.14f / 2, glm::vec3(0.0f, 1.0f, 1.0f));
    model = glm::rotate(model, timePassed * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));
    // model = glm::scale(model,glm::vec3(.2f,.2f,.2f)); //An example of scale
    glUniformMatrix4fv(ShaderManager::Attributes.model, 1, GL_FALSE, glm::value_ptr(model));  // pass model matrix to shader

    // Set which texture to use (-1 = no texture)
    glUniform1i(ShaderManager::Attributes.texID, UNTEXTURED);

    // Draw an instance of the model (at the position & orientation specified by the model matrix above)
    glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts);  //(Primitive Type, Start Vertex, Num Verticies)

    //************
    // Draw model #1 the second time
    // This model is stored in the VBO starting a offset model1_start and with model1_numVerts num. of verticies
    //*************

    // Translate the model (matrix) left and back
    model = glm::mat4();  // Load intensity
    model = glm::translate(model, glm::vec3(-2, -1, -.4));
    // model = glm::scale(model,2.f*glm::vec3(1.f,1.f,0.5f)); //scale example
    glUniformMatrix4fv(ShaderManager::Attributes.model, 1, GL_FALSE, glm::value_ptr(model));

    // Set which texture to use (0 = wood texture ... bound to GL_TEXTURE0)
    glUniform1i(ShaderManager::Attributes.texID, TEX0);

    // Draw an instance of the model (at the position & orientation specified by the model matrix above)
    glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts);  //(Primitive Type, Start Vertex, Num Verticies)

    //************
    // Draw model #2 once
    // This model is stored in the VBO starting a offset model2_start and with model2_numVerts num of verticies
    //*************

    // Translate the model (matrix) based on where objx/y/z is
    // ... these variables are set when the user presses the arrow keys
    model = glm::mat4();
    model = glm::scale(model, glm::vec3(.8f, .8f, .8f));  // scale this model
    model = glm::translate(model, glm::vec3(objx, objy, objz));

    // Set which texture to use (1 = brick texture ... bound to GL_TEXTURE1)
    glUniform1i(ShaderManager::Attributes.texID, TEX1);
    glUniformMatrix4fv(ShaderManager::Attributes.model, 1, GL_FALSE, glm::value_ptr(model));

    // Draw an instance of the model (at the position & orientation specified by the model matrix above)
    glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts);  //(Primitive Type, Start Vertex, Num Verticies)
}
