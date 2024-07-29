#include "Scene.hpp"


namespace Scene {

    Camera::Camera(float aspectRatio) {
        glm::vec3 position = glm::vec3(0, 0, 5); // Camera position
        glm::vec3 target = glm::vec3(0, 0, 0); // Look at target
        glm::vec3 up = glm::vec3(0, 1, 0); // Up vector

        matrix.view = glm::lookAt(position, target, up);
        matrix.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
        matrix.viewProj = matrix.proj * matrix.view;
    }

    glm::vec3 Camera::project(const glm::vec3& worldPoint, float screenWidth, float screenHeight) {
        // Transform the point to NDC space using the viewProj matrix
        glm::vec4 clipSpacePoint = matrix.viewProj * glm::vec4(worldPoint, 1.0);
        glm::vec3 ndcSpacePoint = glm::vec3(clipSpacePoint) / clipSpacePoint.w;

        // Convert NDC to screen space
        float x2D = (ndcSpacePoint.x + 1) * screenWidth / 2;
        float y2D = (1 - ndcSpacePoint.y) * screenHeight / 2; // Y is inverted for screen coordinates

        return glm::vec3(x2D, y2D, ndcSpacePoint.z);
    }

    glm::vec3 Camera::getCameraPosition(glm::mat4& view) {
        // Invert the view matrix to get the transformation matrix of the camera in the world
        glm::mat4 invView = glm::inverse(view);

        // The translation vector of the inverted view matrix is the camera's position in world coordinates
        glm::vec3 cameraPosition = glm::vec3(invView[3]);

        return cameraPosition;
    }



}