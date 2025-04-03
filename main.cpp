#include "main.h"

// Helper Functions

void close()
{
  if (EBO)
    glDeleteBuffers(1, &EBO);
  if (VBO)
    glDeleteBuffers(1, &VBO);
  if (VAO)
    glDeleteVertexArrays(1, &VAO);
  if (shaderProgram)
    glDeleteProgram(shaderProgram);
  if (context)
    SDL_GL_DestroyContext(context);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

void checkShaderCompilation(GLuint shader, const char *type)
{
  GLint success;
  GLchar infoLog[1024];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    std::cerr << "Shader Compilation Error (" << type << "): " << infoLog << std::endl;
  }
}

void checkShaderLinking(GLuint program)
{
  GLint success;
  GLchar infoLog[1024];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 1024, NULL, infoLog);
    std::cerr << "Shader Linking Error: " << infoLog << std::endl;
  }
}

// Main Functions

bool init()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Request OpenGL 3.3 Core Profile (Required on macOS)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // Create Window
  window = SDL_CreateWindow("grid plane", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!window)
  {
    cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
    close();
    return false;
  }

  // Create GL context
  context = SDL_GL_CreateContext(window);
  if (!context)
  {
    cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << endl;
    close();
    return false;
  }

  // Fixing driver Issues
  glewExperimental = GL_TRUE;

  GLenum glErr = glewInit();

  if (glErr != GLEW_OK)
  {
    cerr << "GLEW Init Error: " << glewGetErrorString(glErr) << endl;
    close();
    return false;
  }

  if (!SDL_GL_SetSwapInterval(1))
  {
    cerr << "Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
    return false;
  }

  return true;
}

void loadShader(const char *vertexPath, const char *fragPath)
{
  ifstream vertexFile(vertexPath), fragFile(fragPath);
  stringstream vertexStream, fragStream;

  vertexStream << vertexFile.rdbuf();
  fragStream << fragFile.rdbuf();

  string vertexCode = vertexStream.str();
  string fragCode = fragStream.str();

  const char *vertexStr = vertexCode.c_str();
  const char *fragStr = fragCode.c_str();

  // vertex creation
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexStr, NULL);
  glCompileShader(vertexShader);
  checkShaderCompilation(vertexShader, "vertex");

  // fragment creation
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragStr, NULL);
  glCompileShader(fragShader);
  checkShaderCompilation(fragShader, "fragment");

  // shader binding
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);
  checkShaderLinking(shaderProgram);

  // delete shaders after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);
}

bool initGL()
{
  loadShader("lib/shaders/vertex.glsl", "lib/shaders/frag.glsl");

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBindVertexArray(0);

  return true;
}

void update()
{
  // FPS Update logic
}

void handleKeys(unsigned char key)
{
  // Keys Logic
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT);

  SDL_GL_SwapWindow(window);
}

int main(int argc, char *argv[])
{
  if (!init())
  {
    cerr << "Failed to initialize" << endl;
    return -1;
  }

  if (!initGL())
  {
    cerr << "Failed to initialize OpenGl" << endl;
    return -1;
  }

  SDL_Event evt;
  bool running = false;

  while (!running)
  {
    while (SDL_PollEvent(&evt))
    {
      if (evt.type == SDL_EVENT_QUIT)
      {
        running = true;
      }
    }

    render();
  }

  close();

  return 0;
}