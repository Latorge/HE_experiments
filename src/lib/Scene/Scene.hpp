// createOperations.hpp
#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

#include "../Operations/Operations.hpp"
#include "../CreateOperations/createOperations.hpp"
#include "../Modificators/Modificators.hpp"
#include "../Intersection/intersections.hpp"
#include "../DrawSupport/drawSupport.hpp"
#include "../Core/HalfedgeDS.hpp"

namespace Scene {

    struct CameraMatrix{
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 viewProj; // This is view * proj

    };

    class Camera {
    public:
        CameraMatrix matrix;
        Camera() {};
        Camera(float aspectRatio);
        glm::vec3 project(const glm::vec3& worldPoint, float screenWidth, float screenHeight);
        glm::vec3 getCameraPosition(glm::mat4& view);
        glm::vec3 position; // Position of the camera in world space
    };

    class Scene {
    private:
        std::vector<HalfedgeDS*> halfedgeDSList; // Pointer to the mesh
    
    public:
        Camera camera; // Pointer to the camera
        Scene() {}
        
        void updateFrontFaces(HalfedgeDS& structD);
        const std::vector<Face*>& getFrontFaces() const;
        void setCameraPosition(glm::vec3 pos);

        void testCube(HalfedgeDS &halfedgeDS);
        void testNautilus(HalfedgeDS &halfedgeDS);
        void fillHalfCirclePoints(std::vector<glm::vec3> &sourcePoints, int N, float radius);
        void fillSpiralPoints(std::vector<glm::vec3> &sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops);
        void fillSpiralPointsNAU(std::vector<glm::vec3> &sourcePoints, int N, float initialRadius, float finalRadius, int numberOfLoops, float radiusCoeff);
    };
}

#endif // SCENE_HPP
