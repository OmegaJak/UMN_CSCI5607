// Multi-Object, Multi-Texture Example
// Stephen J. Guy, 2018

// This example demonstrates:
// Loading multiple models (a cube and a knot)
// Using multiple textures (wood and brick)
// Instancing (the teapot is drawn in two locations)
// Continuous keyboard input - arrows (moves knot up/down/left/right continuous on being held)
// Keyboard modifiers - shift (up/down arrows move knot in/out of screen when shift is pressed)
// Single key events - pressing 'c' changes color of a random teapot
// Mixing textures and colors for models
// Phong lighting
// Binding multiple textures to one shader

#include "bounding_box.h"
#include "camera.h"
#include "constants.h"
#include "game_object.h"
#include "map.h"
#include "map_loader.h"
#include "player.h"
#include "shader_manager.h"
#include "texture_manager.h"
const char* INSTRUCTIONS =
    "***************\n"
    "This demo shows multiple objects being draw at once along with user interaction.\n"
    "\n"
    "Up/down/left/right - Moves the knot.\n"
    "c - Changes to teapot to a random color.\n"
    "***************\n";

const char* USAGE =
    "Usage:\n"
    "-w \'width\'x\'height\'\n"
    "   Example: -m 800x600\n"
    "-m map\n"
    "   This map must be in the root of the directory the game's being run from.\n"
    "   Example: -m map1.txt\n";

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

#include "map.h"
#include "model_manager.h"

using namespace std;

int screenWidth = 1536;
int screenHeight = 864;
float timePassed = 0;

bool fullscreen = false;

// srand(time(NULL));
float rand01() {
    return rand() / (float)RAND_MAX;
}

void drawGeometry(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts);

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    bool window_size_specified = false;
    std::string map_file = "map2.txt";
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'w':
                    int result = sscanf_s(argv[++i], "%ix%i", &screenWidth, &screenHeight);
                    if (result == 2) {
                        window_size_specified = true;
                    } else {
                        printf("%s\n", USAGE);
                        exit(1);
                    }
                    break;
                case 'm':
                    map_file = argv[++i];
                    break;
                default:
                    printf("%s\n", USAGE);
                    exit(1);
            }
        }
    }

    SDL_Init(SDL_INIT_VIDEO);  // Initialize Graphics (for OpenGL)

    // Ask SDL to get a recent version of OpenGL (3.2 or greater)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create a window (offsetx, offsety, width, height, flags)
    SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

    // Maximize the window if no size was specified
    if (!window_size_specified) {
        SDL_SetWindowResizable(window, SDL_TRUE);                // Allow resizing
        SDL_MaximizeWindow(window);                              // Maximize
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);  // Get the new size
        SDL_SetWindowResizable(window, SDL_FALSE);               // Disable future resizing
    }

    // Create a context to draw in
    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_SetRelativeMouseMode(SDL_TRUE);  // 'grab' the mouse

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

    MapLoader map_loader;
    Map* map = map_loader.LoadMap(map_file);
    Camera camera = Camera();

    Player player(&camera, map);
    map->Add(&player);

    // Load the textures
    TextureManager::InitTextures();

    // Build a Vertex Array Object (VAO) to store mapping of shader attributes to VBO
    GLuint vao;
    glGenVertexArrays(1, &vao);  // Create a VAO
    glBindVertexArray(vao);      // Bind the above created VAO to the current context

    ModelManager::InitVBO();

    ShaderManager::InitShader("textured-Vertex.glsl", "textured-Fragment.glsl");

    TextureManager::InitTextures();

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
            if (windowEvent.type == SDL_KEYUP) {  // Exit event loop
                if (windowEvent.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                } else if (windowEvent.key.keysym.sym == SDLK_F11) {  // If F11 is pressed
                    fullscreen = !fullscreen;
                    SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);  // Toggle fullscreen
                } else if (windowEvent.key.keysym.sym == SDLK_LCTRL) {
                    player.UnCrouch();
                } else if (windowEvent.key.keysym.sym == SDLK_g) {
                    player.DropKey();
                }
            }
            if (windowEvent.type == SDL_KEYDOWN) {
                if (windowEvent.key.keysym.sym == SDLK_SPACE) {
                    player.Jump();
                } else if (windowEvent.key.keysym.sym == SDLK_LCTRL) {
                    player.Crouch();
                }
            }

            if (windowEvent.type == SDL_MOUSEMOTION && SDL_GetRelativeMouseMode() == SDL_TRUE) {
                // printf("Mouse movement (xrel, yrel): (%i, %i)\n", windowEvent.motion.xrel, windowEvent.motion.yrel);
                float factor = 0.002f;
                camera.Rotate(0, -windowEvent.motion.xrel * factor);
            }

            switch (windowEvent.window.event) {
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    SDL_Log("Window focus lost");
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    SDL_Log("Window focus gained");
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    break;
            }
        }

        // Clear the screen to default color
        glClearColor(.2f, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ShaderManager::Textured_Shader);

        timePassed = SDL_GetTicks() / 1000.f;

        player.Update();
        camera.Update();

        glm::mat4 proj = glm::perspective(3.14f / 2, screenWidth / (float)screenHeight, 0.01f, 1000.0f);  // FOV, aspect, near, far
        glUniformMatrix4fv(ShaderManager::Attributes.projection, 1, GL_FALSE, glm::value_ptr(proj));

        TextureManager::Update();

        glBindVertexArray(vao);

        map->UpdateAll();

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
