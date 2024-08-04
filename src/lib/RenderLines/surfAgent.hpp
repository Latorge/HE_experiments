// Operations.hpp
#ifndef SURFAGENT_HPP
#define SURFAGENT_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

#include "../DrawSupport/drawSupport.hpp"
#include "../Scene/Scene.hpp"
#include "surfSupportFunctions.hpp"

#include <unordered_set>


namespace RenderLines {

    class SurfAgent {
    public:
        SurfAgent(glm::vec3 startPosition,
            Face* startFace,
            glm::vec2 initialVelocity,
            size_t maxTrailLength = 1100);
        void initialize2DEdges(Face *face);
       
        //void update3d(float deltaTime);

        std::tuple<Edge2D, bool, glm::vec2> crossesEdges(const glm::vec2 &startPos, const glm::vec2 &endPos);

        bool pointOnEdge(const glm::vec2 &point, const glm::vec2 &start, const glm::vec2 &end);

        std::tuple<Edge2D, bool, bool, glm::vec2> crossesEdgesRay(const glm::vec2& startPos, const glm::vec2& endPos,float deltaTime);

        void update(float deltaTime);
        void update2D(float _deltaTime);

        void adjustVelocityDirection3D(glm::vec3 &velocity, const glm::vec3 &fromNormal, const glm::vec3 &toNormal);

        void adjustVelocity2DDirection(glm::vec2 &velocity2D, Face *face,const glm::vec3 &fromNormal, Face *face2,const glm::vec3 &toNormal);

        void adjustAgentVelocity(Face *face1, Face *face2, Halfedge* edge1, Halfedge* edge2, glm::vec2 &velocity2D);

        void adjustVelocityDirection(glm::vec2 &velocity2D, Face *oldFace, Face *newFace, float deltaTime);

        bool isPositionWithinTriangle(const glm::vec2 &position, Face *face);

        void adjustVelocityDirectionNC(glm::vec2 &velocity2D, Face *face, const glm::vec3 &oldNormal, const glm::vec3 &newNormal);

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
        const std::deque<std::pair<glm::vec2, Face*>>& getFaceTrail() const { return trailFace; }
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
        std::deque<std::pair<glm::vec2, Face*>> trailFace;
        size_t maxTrailLength;

        std::vector<Edge2D> edges2D;

        float deltaTime;
    };


    
}

#endif // SURFAGENT_HPP
