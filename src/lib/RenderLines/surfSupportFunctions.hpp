// Operations.hpp
#ifndef SURFSUPPORTFUNCTIONS_HPP
#define SURFSUPPORTFUNCTIONS_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp> // For glm::proj

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

#include "../DrawSupport/drawSupport.hpp"
#include "../Scene/Scene.hpp"

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

    glm::vec3 localToBarycentric(const glm::vec2 &localPos, Face *face);

    void transferVelocityUsingBarycentric(glm::vec2 &velocity2D, Face *oldFace, Face *newFace, glm::vec2 &localPosition);

    float timeToIntersectEdge(const glm::vec3& startPos, const glm::vec3& endPos, Halfedge* edge);
    float timeToIntersectEdge(const glm::vec2 &startPos, const glm::vec2 &endPos, const glm::vec2 &edgeStart, const glm::vec2 &edgeEnd);
    void adjustVelocityDirection(glm::vec3 &velocity, const glm::vec3 &currentNormal, const glm::vec3 &nextNormal);

    IntersectionResult rayIntersectEdge(const glm::vec2& startPos, const glm::vec2& velocity, const Edge2D& edge) ;

    glm::vec3 projectPointOntoPlane(const glm::vec3 &point, const glm::vec3 &planeNormal, const glm::vec3 &planePoint);

    glm::vec3 projectVelocityOntoPlane(const glm::vec3 &velWorld, Face *face);

    glm::vec3 projectPointOntoPlane(const glm::vec3 &point, Face *face);

    glm::vec3 vectorLerp(const glm::vec3 &start, const glm::vec3 &end, float t);

    glm::vec3 calculateNormal(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

    glm::quat rotationBetweenTriangles(const glm::vec3 &fromNormal, const glm::vec3 &toNormal);

    glm::quat calculateRotationBetweenFaces(Face *fromFace, Face *toFace);

    float calculateVelocityAngle(const glm::vec2 &velocity, Halfedge* edge);
    glm::vec2 constructVelocityFromAngle(float angle, float magnitude, Face *face, Halfedge* edge);

    glm::vec3 calculate3DVelocity(const glm::vec2 &velocity2D, const glm::vec2 &position2D, Face *face);

    glm::vec3 rotateVector(const glm::vec3 &vec, const glm::vec3 &axis, float angle);

    float angleBetweenFaces(Face *face1, Face *face2);

    bool isApproachingVertex(const glm::vec2 &pos, Face *face);

    glm::vec3 safeNormalize(const glm::vec3 &v);

    glm::vec3 convert2DVelocityTo3D(const glm::vec2 &velocity2D, Face *face);

    
}

#endif // SURFSUPPORTFUNCTIONS_HPP
