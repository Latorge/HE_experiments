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

    struct Edge2D {
        glm::vec2 start;
        glm::vec2 end;
        Halfedge* halfedge;
    };

    struct ProjectionAxes {
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    struct IntersectionResult {
        bool intersect=false;
        glm::vec2 point;
        float distance;
    };



    glm::vec3 sphericalRand(float radius);

    glm::vec2 circularRand(float radius);

    ProjectionAxes getProjectionAxes(Face *face);
    glm::vec2 projectTo2D(const glm::vec3 &pos3D, Face *currentFace);
    glm::vec3 projectTo3D(const glm::vec2 &pos2D, Face *face);

    bool crossesEdge(const glm::vec3& startPos, const glm::vec3& endPos, Halfedge* edge);
    bool crossesEdge(const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& edgeStart, const glm::vec2& edgeEnd);

    float timeToIntersectEdge(const glm::vec3& startPos, const glm::vec3& endPos, Halfedge* edge);
    float timeToIntersectEdge(const glm::vec2 &startPos, const glm::vec2 &endPos, const glm::vec2 &edgeStart, const glm::vec2 &edgeEnd);
    void adjustVelocityDirection(glm::vec3 &velocity, const glm::vec3 &currentNormal, const glm::vec3 &nextNormal);

    IntersectionResult rayIntersectEdge(const glm::vec2& startPos, const glm::vec2& velocity, const Edge2D& edge) ;

    glm::vec3 localToBarycentric(const glm::vec2 &localPos);

    class SurfAgent {
    public:
        SurfAgent(glm::vec3 startPosition,
            Face* startFace,
            glm::vec2 initialVelocity,
            size_t maxTrailLength = 100);

        void initialize2DEdges(Face *face);
       

        //void update3d(float deltaTime);

        std::tuple<Edge2D, bool, glm::vec2> crossesEdges(const glm::vec2 &startPos, const glm::vec2 &endPos);

        bool pointOnEdge(const glm::vec2 &point, const glm::vec2 &start, const glm::vec2 &end);

        std::tuple<Edge2D, bool, glm::vec2> crossesEdgesRay(const glm::vec2& startPos, const glm::vec2& endPos);

        void update(float deltaTime);

        void adjustVelocityDirection(glm::vec2 &velocity2D, Face *face, const glm::vec3 &oldNormal, const glm::vec3 &newNormal);


        void handleOpenBoundary3D();
        void handleOpenBoundary(const glm::vec2& edgeStart, const glm::vec2& edgeEnd);

        void changeFace(Halfedge* edge);

        void render() const;

        glm::vec3 barycentricToPoint(Face* face, const glm::vec3& bary) const;

        void updateBarycentric();
        void updateWorldPosToLocalPos();

        glm::vec3 updateBarycentric(Face* face, const glm::vec3& pos);

        glm::vec3 globalToBarycentric(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p) const;

        const glm::vec3& getPosition3D() const { return posWorld; }
        const glm::vec2& getPosition2D() const { return posLocal; }
        const glm::vec3& getBarycentric() const { return barycentric; }
        const std::deque<glm::vec3>& getTrail() const { return trail; }
        const size_t getMaxTrailLength() const {return maxTrailLength; }

        Face* getCurrentFace() const {return currentFace;}
        Face* getNextFace() const {return nextFace;}

    private:

        Face* currentFace=nullptr;
        Face* nextFace=nullptr;

        glm::vec3 posWorld;
        glm::vec3 velWorld;

        glm::vec3 barycentric;

        glm::vec2 posLocal;  // Local 2D position on the triangle's plane
        glm::vec2 velLocal;  // Local 2D velocity on the triangle's plan

        std::deque<glm::vec3> trail;
        size_t maxTrailLength;

        std::vector<Edge2D> edges2D;

        float deltaTime;
    };

 

    class SurfRender {
    public:
        SurfRender(HalfedgeDS* halfedgeDS, int initNumberAgents);

        void initializeAgents(int count);

        void update(float deltaTime);

        void render();

        std::vector<DrawSupport::PointInfo> collectTrailSegments();

        bool isAtBoundary(const SurfAgent &agent);

        std::vector<DrawSupport::PointInfo> drawProjectedAgentsAndFaces(float planeZ);

        glm::vec3 convertTo3D(const glm::vec2 &point2D, float planeZ);
        std::vector<DrawSupport::PointInfo> drawSmallerTriangle(Face *face, float scale, float planeZ, glm::vec3 faceColor);

    private:
        HalfedgeDS* halfedgeDS;
        std::vector<SurfAgent> agents;

        // Example function to check if an agent is at a boundary
      
    };


   
    
}

#endif // SURFSPRITERENDER_HPP