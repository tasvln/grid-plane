#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 768;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_GLContext context;

// Open GL vars
GLuint shaderProgram = 0;
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;

void close();