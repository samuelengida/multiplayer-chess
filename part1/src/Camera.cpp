#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

Camera& Camera::Instance(){
    static Camera* instance = new Camera();
    return *instance;
}

void Camera::MouseLook(int mouseX, int mouseY){
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);
    // Detect how much the mouse has moved since
    // the last time
    glm::vec2 mouseDelta = 0.01f*(newMousePosition-m_oldMousePosition);

    m_viewDirection = glm::mat3(glm::rotate(-mouseDelta.x, m_upVector)) * m_viewDirection;
    
    // Update our old position after we have made changes 
    m_oldMousePosition = newMousePosition;
}

// OPTIONAL TODO: 
//               The camera could really be improved by
//               updating the eye position along the m_viewDirection.
//               Think about how you can do this for a better camera!

void Camera::MoveForward(float speed){
    m_eyePosition.z -= speed;
}

void Camera::MoveBackward(float speed){
    m_eyePosition.z += speed;
}

void Camera::MoveLeft(float speed){
    m_eyePosition.x -= speed;
}

void Camera::MoveRight(float speed){
    m_eyePosition.x += speed;
}

void Camera::MoveUp(float speed){
    m_eyePosition.y += speed;
}

void Camera::MoveDown(float speed){
    m_eyePosition.y -= speed;
}

float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}

void Camera::revolve( int turn) {
    static float rotate = 0.0f;
    // For marble pice turn
    if (turn == 0) {
        m_eyePosition = glm::vec3(9.0f,5.0f, -2.5f);
        m_viewDirection = glm::vec3(-1.5f,-1.0f, 0.0f); 
    } else {
        // For white chess piece turn
        m_eyePosition = glm::vec3(-4.0f,5.0f, -2.5f);
        m_viewDirection = glm::vec3(1.5f,-1.0f, 0.0f);
    }
}

Camera::Camera(){
    std::cout << "Created a Single Camera!\n";
	// Position us at the origin.
    m_eyePosition = glm::vec3(9.0f,5.0f, -2.5f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    m_viewDirection = glm::vec3(-1.5f,-1.0f, 0.0f);
	// For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
