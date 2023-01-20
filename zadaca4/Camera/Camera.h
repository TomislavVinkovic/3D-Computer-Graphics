#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <iostream>
#include "Movement.h"

struct Camera {

    glm::vec3 Position = glm::vec3(16.f, 16.f, 32.f);
    glm::vec3 Front = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    //matrica projekcije
    glm::mat4 ProjectionMatrix = glm::mat4(1.f);

    //eulerovi kutevi
    float Yaw = 90.f;
    float Pitch = 0.f;

    //options
    float MovementSpeed = 10.f;
    float MouseSensitivity = 0.1f;
    float Zoom = 45.f;

    Camera(unsigned int scr_width, unsigned int scr_height)
        : Camera(45.f, scr_width, scr_height, 0.1f, 1000.f){}

    Camera(
      float FOV, unsigned int scr_width, unsigned int scr_height, float zNear, float zFar,
      const glm::vec3& position = glm::vec3(1.f, 2.f, -4.f),
      const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f)
    ) : Position(position), WorldUp(up) {
        Zoom = FOV;
        ProjectionMatrix = glm::perspective(FOV, (float)scr_width/(float)scr_height, zNear, zFar);
        updateCameraVectors(true);
    }

    Camera(
        float FOV, unsigned int scr_width, unsigned int scr_height, float zNear, float zFar,
        const glm::mat4& ProjectionMatrix,
        const glm::vec3& position,
        const glm::vec3& up
    ) {
        this->ProjectionMatrix = glm::perspective(FOV, (float)scr_width/(float)scr_height, zNear, zFar);
        this->Position = position;
        this->WorldUp = up;
        updateCameraVectors(true);
        glm::vec3 Front = glm::vec3(0.f, 0.f, -1.f);
    }

    //getters
    glm::mat4 GetViewMatrix() const{
        glm::mat4 view = glm::lookAt(Position, Position + Front, Up);
        return view;
    }
    glm::mat4 GetProjectionMatrix() const {
        return ProjectionMatrix;
    }

    //setters
    void SetProjectionMatrix(
        float FOV, unsigned int scr_width, unsigned int scr_height, float zNear, float zFar
    ) {
        ProjectionMatrix = glm::perspective(FOV, (float)scr_width/(float)scr_height, zNear, zFar);
    }

    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

    private:
        void updateCameraVectors(bool init = false);
};