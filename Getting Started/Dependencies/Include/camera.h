#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum camera_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVITY = 0.5f;
const float FOV = 45.0f;

const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);

class Camera {
    public:

        //camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;

        float MovementSpeed;
        float MouseSensitivity;
        float Fov;
        
        //constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(FRONT), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV) {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            UpdateCameraVectors();
        }

        //cosntructor with floats
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = YAW, float pitch = PITCH) : Front(FRONT), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV) {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            UpdateCameraVectors();
        }

        //function to return view matrix
        glm::mat4 getViewMatrix() {
            return glm::lookAt(Position, Position + Front, Up);
        }

        void processKeyboardInput(camera_movement direction, float deltaTime) {
            float velocity = MovementSpeed * deltaTime;

            if (direction == FORWARD)
                Position += glm::vec3(Front.x, 0, Front.z) * velocity;
            if (direction == BACKWARD)
                Position -= glm::vec3(Front.x, 0, Front.z) * velocity;
            if (direction == RIGHT)
                Position += glm::vec3(Right.x, 0, Right.z) * velocity;
            if (direction == LEFT) 
                Position -= glm::vec3(Right.x, 0, Right.z) * velocity;
            if (direction == JUMP)
                jumping = true;
        }

        void jump(float jumpHeight, float gForce, float deltaTime) {
            if (jumping) {
                Position.y += gForce * deltaTime;
                if (Position.y >= jumpHeight) {
                    Position.y = jumpHeight;
                    jumping = false;
                    falling = true;
                }
            } else if (falling) {
                Position.y -= gForce * deltaTime; 
                if (Position.y <= 0) {
                    Position.y = 0;
                    falling = false;
                }
            }
        }

        void processMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true) {

            xOffset *= MouseSensitivity;
            yOffset *= MouseSensitivity;

            Yaw += xOffset;
            Pitch -= yOffset;

            if (constrainPitch) {
                if(Pitch > 89.0f)
                    Pitch = 89.0f;
                if(Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            UpdateCameraVectors();
        }

        void processMouseScroll(double yOffset) {
            Fov -= yOffset;

            if(Fov < 1.0f)
                Fov = 1.0f;
            if(Fov > 45.0f)
                Fov = 45.0f;
        }

    private:

        bool jumping = false;
        bool falling = false;

        void UpdateCameraVectors() {
            glm::vec3 front;
            front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
            front.y = sin(glm::radians(Pitch));
            front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
            Front = glm::normalize(front);

            Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));
        }
};
#endif