// createOperations.hpp
#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

#include "../Operations/Operations.hpp"

namespace Scene {

    struct CameraMatrix{
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 viewProj; // This is view * proj

    };

    class Camera {
    public:
       
        CameraMatrix matrix;

    public:
        Camera() {};
        Camera(float aspectRatio) {
            glm::vec3 position = glm::vec3(0, 0, 5); // Camera position
            glm::vec3 target = glm::vec3(0, 0, 0); // Look at target
            glm::vec3 up = glm::vec3(0, 1, 0); // Up vector

            matrix.view = glm::lookAt(position, target, up);
            matrix.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
            matrix.viewProj = matrix.proj * matrix.view;
        }

        glm::vec3 project(const glm::vec3& worldPoint, float screenWidth, float screenHeight) {
            // Transform the point to NDC space using the viewProj matrix
            glm::vec4 clipSpacePoint = matrix.viewProj * glm::vec4(worldPoint, 1.0);
            glm::vec3 ndcSpacePoint = glm::vec3(clipSpacePoint) / clipSpacePoint.w;

            // Convert NDC to screen space
            float x2D = (ndcSpacePoint.x + 1) * screenWidth / 2;
            float y2D = (1 - ndcSpacePoint.y) * screenHeight / 2; // Y is inverted for screen coordinates

            return glm::vec3(x2D, y2D, ndcSpacePoint.z);
        }

        glm::vec3 getCameraPosition(glm::mat4& view) {
            // Invert the view matrix to get the transformation matrix of the camera in the world
            glm::mat4 invView = glm::inverse(view);

            // The translation vector of the inverted view matrix is the camera's position in world coordinates
            glm::vec3 cameraPosition = glm::vec3(invView[3]);

            return cameraPosition;
        }

        glm::vec3 position; // Position of the camera in world space
    };

    class Scene {
    private:
        std::vector<Face*> frontFaces; // Vector to store front faces
        //HalfedgeDS* mesh; // Pointer to the mesh
        

    public:
        Camera camera; // Pointer to the camera
        Scene() {}
        
        void updateFrontFaces(HalfedgeDS& structD) {
            frontFaces.clear();
            // Loop through all faces in the mesh
            for (Face* face :structD.getFaces() ) {
                if (face->isFront(camera.position)) {
                    frontFaces.push_back(face); // Add to front faces if facing towards the camera
                }
            }
        }

        const std::vector<Face*>& getFrontFaces() const {
            return frontFaces;
        }

        void setCameraPosition(glm::vec3 pos)
        {
            camera.position=pos;
        }
    };



}

#endif // SCENE_HPP
