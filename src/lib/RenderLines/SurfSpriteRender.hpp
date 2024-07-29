// Operations.hpp
#ifndef SURFSPRITERENDER_HPP
#define SURFSPRITERENDER_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <random>

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/range.hpp>
#include <cmath>

#include "../DrawSupport/drawSupport.hpp"
#include "../Scene/Scene.hpp"
#include "RenderLines.hpp"

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

#include <unordered_set>


namespace RenderLines {


    inline glm::vec3 sphericalRand(float radius) {
        float z = 2.0f * ((float) rand() / RAND_MAX) - 1.0f; // Random z between -1 and 1
        float t = 2.0f * M_PI * ((float) rand() / RAND_MAX); // Random theta
        float r = sqrt(1.0f - z * z); // Radius for x and y
        float x = r * cos(t);
        float y = r * sin(t);
        return glm::vec3(x, y, z) * radius;
    }


    bool crossesEdge(const glm::vec3& startPos, const glm::vec3& endPos, Halfedge* edge);
    float timeToIntersectEdge(const glm::vec3& startPos, const glm::vec3& endPos, Halfedge* edge);
    void adjustVelocityDirection(glm::vec3& velocity,const glm::vec3& currentNormal, const glm::vec3& nextNormal);

    class SurfAgent {
    public:
        SurfAgent(glm::vec3 startPosition,
         Face* startFace,
         glm::vec3 initialVelocity,
         size_t maxTrailLength = 100)
        : position(startPosition), currentFace(startFace), velocity(initialVelocity), maxTrailLength(maxTrailLength) {}

        void update(float deltaTime);

        void handleOpenBoundary();

        void changeFace(Halfedge* edge);

        void render() const;

        glm::vec3 barycentricToPoint(Face* face, const glm::vec3& bary) const;

        void updateBarycentric();

        glm::vec3 updateBarycentric(Face* face, const glm::vec3& pos);

        glm::vec3 globalToBarycentric(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p) const;

        const glm::vec3& getPosition() const { return position; }
        const glm::vec3& getBarycentric() const { return barycentric; }
        const std::deque<glm::vec3>& getTrail() const { return trail; }
        const size_t getMaxTrailLength() const {return maxTrailLength; }

    private:
        glm::vec3 position;
        glm::vec3 barycentric;
        Face* currentFace;
        glm::vec3 velocity;
        //HalfedgeDS& halfedgeDS;
        std::deque<glm::vec3> trail;
        size_t maxTrailLength;
    };

 

    class SurfRender {
    public:
        SurfRender(HalfedgeDS* halfedgeDS, int initNumberAgents);

        void initializeAgents(int count);

        void update(float deltaTime);

        void render();

        std::vector<DrawSupport::PointInfo> collectTrailSegments();

        bool isAtBoundary(const SurfAgent &agent);

    private:
        HalfedgeDS* halfedgeDS;
        std::vector<SurfAgent> agents;

        // Example function to check if an agent is at a boundary
      
    };


   
    
}

#endif // SURFSPRITERENDER_HPP