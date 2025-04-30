#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO: split camera's -> Orbit Camera & FPS Camera & TPS Camera

enum Camera_Movement
{
  UP,
  DOWN,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class Camera
{
public:
  // Basic Camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  // Orbit Attributes
  glm::vec3 Target;

  // Euler Angles
  float Yaw;
  float Pitch;

  // Orbit Attributes
  float Radius;

  // Basic Camera Options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // Constructor with default values
  Camera(
      glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
      float yaw = -90.0f,
      float pitch = 0.0f) : Front(glm::vec3(0.0f, 0.0f, 1.0f)),
                            MovementSpeed(2.5f),
                            MouseSensitivity(0.1f),
                            Zoom(45.0f)
  {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors(); // Important: calculates Right and Up based on Front
  }

  glm::mat4 GetViewMatrix()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime)
  {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
      Position += Front * velocity;
    if (direction == BACKWARD)
      Position -= Front * velocity;
    if (direction == LEFT)
      Position -= Right * velocity;
    if (direction == RIGHT)
      Position += Right * velocity;
    if (direction == UP)
      Position.y += velocity;
    if (direction == DOWN)
      Position.y -= velocity;
  }

  void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
  {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Constrain pitch to avoid flipping
    if (constrainPitch)
    {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }

    updateCameraVectors();
  }

  void ProcessMouseScroll(float yoffset)
  {
    Zoom -= yoffset;
    if (Zoom < 1.0f)
      Zoom = 1.0f;
    if (Zoom > 45.0f)
      Zoom = 45.0f;
  }

private:
  void updateCameraVectors()
  {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

#endif
