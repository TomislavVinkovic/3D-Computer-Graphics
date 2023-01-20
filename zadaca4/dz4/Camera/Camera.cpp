#include "Camera.h"

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if(direction == Movement::FORWARD) {
        Position += Front * velocity;
    }
    if(direction == Movement::BACKWARD) {
        Position -= Front * velocity;
    }
    if(direction == Movement::LEFT){
        Position -= Right * velocity;
    }
    if(direction == Movement::RIGHT) {
        Position += Right * velocity;
    }
    //std::cout << Position.x << " " << Position.y << " " << Position.z << std::endl;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {

    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if(constrainPitch) {
        if(Pitch > 89.f) Pitch = 89.f;
        if(Pitch < -89.f) Pitch = -89.f;
    }
    updateCameraVectors();
}

void Camera::updateCameraVectors(bool init) {
    if(!init) {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
    }

    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
    //std::cout << "Front: " << Front.x << " " << Front.y << " " << Front.z << std::endl;

}