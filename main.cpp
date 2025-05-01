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
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_STATIC_DRAW);

  // element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pIndices), pIndices, GL_STATIC_DRAW);

  // TO DO: fully understand how this works
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  return true;
}

void update()
{
  // FPS Update logic
}

void handleKeys(SDL_Scancode key, float deltaTime)
{
}

void render()
{
  glUseProgram(shaderProgram);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = orbitCam.getViewMatrix();
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

  GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
  GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
  GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glUniform1f(glGetUniformLocation(shaderProgram, "spacing"), 5.0f);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void handleOrbitMouseMovement(SDL_Event event)
{
  static int lastX = SCREEN_WIDTH / 2;
  static int lastY = SCREEN_HEIGHT / 2;
  static bool firstMouse = true;

  if (event.type == SDL_EVENT_MOUSE_MOTION)
  {
    SDL_Keymod mod = SDL_GetModState();
    bool ctrlDown = (mod & SDL_KMOD_CTRL);
    bool cmdDown = (mod & SDL_KMOD_GUI); // Command key on macOS

    if (!(ctrlDown || cmdDown))
      return; // Skip orbiting unless Ctrl or Command is held

    int xpos = event.motion.x;
    int ypos = event.motion.y;

    if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from top to bottom

    lastX = xpos;
    lastY = ypos;

    orbitCam.processMouseMovement(xoffset, yoffset);
  }
}

int main(int argc, char *argv[])
{
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  SDL_SetHint(SDL_HINT_TRACKPAD_IS_TOUCH_ONLY, "1");

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

  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  SDL_Event evt;
  bool running = true;

  int lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
  bool firstMouse = true;

  while (running)
  {
    float currentFrame = SDL_GetTicks() / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    while (SDL_PollEvent(&evt))
    {
      if (evt.type == SDL_EVENT_QUIT)
      {
        running = false;
      }
      if (evt.type == SDL_EVENT_KEY_DOWN)
      {
        handleKeys(evt.key.scancode, deltaTime);
      }
      handleOrbitMouseMovement(evt);
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render();

    SDL_GL_SwapWindow(window);
  }

  close();

  return 0;
}