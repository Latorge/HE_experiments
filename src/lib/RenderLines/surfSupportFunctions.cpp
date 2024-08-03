#include "SurfSpriteRender.hpp"

namespace RenderLines {


    ProjectionAxes getProjectionAxes(Face* face) {
        auto vertices = face->getVertices();
        glm::vec3 origin = vertices[0]->position;
        glm::vec3 edge1 = vertices[1]->position - origin;
        glm::vec3 edge2 = vertices[2]->position - origin;

        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        glm::vec3 tangent = glm::normalize(edge1);
        glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));

        return {tangent, bitangent};
    }

    glm::vec2 projectTo2D(const glm::vec3& pos3D, Face* currentFace) {
        ProjectionAxes axes = getProjectionAxes(currentFace);
        glm::vec3 origin = currentFace->getVertices()[0]->position;
        glm::vec3 relPos = pos3D - origin;
        
        float x = glm::dot(relPos, axes.tangent);
        float y = glm::dot(relPos, axes.bitangent);
        
        return glm::vec2(x, y);
    }

    glm::vec3 projectTo3D(const glm::vec2& pos2D, Face* face) {
        auto vertices = face->getVertices();
        if (vertices.size() < 3) {
            // Not enough vertices to define a plane
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }

        // Define the local coordinate system based on the first two edges of the triangle
        glm::vec3 origin = vertices[0]->position;  // Use the first vertex as the origin
        glm::vec3 edge1 = vertices[1]->position - origin; // First edge
        glm::vec3 edge2 = vertices[2]->position - origin; // Second edge

        // Create orthogonal vectors in the plane
        glm::vec3 localX = glm::normalize(edge1); // Use the first edge as the local X axis
        glm::vec3 planeNormal = glm::cross(edge1, edge2); // Normal of the plane
        glm::vec3 localY = glm::normalize(glm::cross(planeNormal, localX)); // Construct local Y axis orthogonal to X

        // Convert pos2D back to a 3D point using the local axes and origin
        glm::vec3 pos3D = origin + localX * pos2D.x + localY * pos2D.y;

        return pos3D;
    }



    glm::vec3 localToBarycentric(const glm::vec2& localPos) {
            // Assume localPos maps directly to barycentric coordinates for simplicity
            return glm::vec3(localPos.x, localPos.y, 1.0f - localPos.x - localPos.y);
        }

    bool crossesEdge(const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& edgeStart, const glm::vec2& edgeEnd) {

        glm::vec2 edgeDir = edgeEnd - edgeStart;
        glm::vec2 normal2D(-edgeDir.y, edgeDir.x);
        
        float startProj = glm::dot(startPos - edgeStart, normal2D);
        float endProj = glm::dot(endPos - edgeStart, normal2D);
        
        if (startProj * endProj < 0) {
            return true; // Crosses the edge
        }
        
        return false;
    }

    glm::vec3 sphericalRand(float radius) {
        float z = 2.0f * ((float) rand() / RAND_MAX) - 1.0f; // Random z between -1 and 1
        float t = 2.0f * M_PI * ((float) rand() / RAND_MAX); // Random theta
        float r = sqrt(1.0f - z * z); // Radius for x and y
        float x = r * cos(t);
        float y = r * sin(t);
        return glm::vec3(x, y, z) * radius;
    }

    glm::vec2 circularRand(float radius) {
        float t = 2.0f * M_PI * ((float) rand() / RAND_MAX); // Random theta
        float x = cos(t);
        float y = sin(t);
        return glm::vec2(x, y) * radius;
    }




    bool crossesEdge(const glm::vec3& startPos, const glm::vec3& endPos, Halfedge* edge) {
        if (!edge || !edge->vertex || !edge->next || !edge->next->vertex) return false;

        // Retrieve the vertices of the edge
        glm::vec3 edgeStart = edge->vertex->position;
        glm::vec3 edgeEnd = edge->next->vertex->position;

        // Direction of the edge
        glm::vec3 edgeDir = edgeEnd - edgeStart;
        glm::vec3 dir = endPos - startPos;

        // Normal to the plane containing the edge and perpendicular to the movement direction
        glm::vec3 planeNormal = glm::cross(edgeDir, dir);

        // Check if the plane normal is zero (parallel lines), no intersection possible
        if (glm::length(planeNormal) < 1e-8) return false;

        // Determine if the path crosses the edge by checking if the start and end points are on opposite sides of the edge's plane
        glm::vec3 startVec = startPos - edgeStart;
        glm::vec3 endVec = endPos - edgeStart;
        float startDist = glm::dot(glm::cross(edgeDir, startVec), planeNormal);
        float endDist = glm::dot(glm::cross(edgeDir, endVec), planeNormal);

        return startDist * endDist < 0; // Different signs mean they are on opposite sides
    }

    float timeToIntersectEdge(const glm::vec3& startPos, const glm::vec3& endPos, Halfedge* edge) {
        if (!edge || !edge->vertex || !edge->next || !edge->next->vertex) return -1.0f;

        glm::vec3 edgeStart = edge->vertex->position;
        glm::vec3 edgeEnd = edge->next->vertex->position;

        // Using GLM's built-in intersection functions
        glm::vec3 dir = endPos - startPos;
        glm::vec2 baryPosition;
        float distance;

        // Check if the segment intersects the edge
        bool intersected = glm::intersectRayTriangle(startPos, dir, edgeStart, edgeEnd, edgeStart, baryPosition, distance);

        if (intersected && distance >= 0 && distance <= 1) {
            return distance;
        }

        return -1.0f; // Return -1 to indicate no intersection within the segment bounds
    }

    float timeToIntersectEdge(const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& edgeStart, const glm::vec2& edgeEnd) {
        glm::vec2 dir = endPos - startPos;
        glm::vec2 edgeDir = edgeEnd - edgeStart;

        // Calculate determinants
        float denominator = dir.x * edgeDir.y - dir.y * edgeDir.x;
        
        // Check if lines are parallel
        if (std::abs(denominator) < 1e-8) {
            return -1.0f; // Lines are parallel or coincident
        }

        // Find the intersection parameter t for the line segment [startPos, endPos]
        glm::vec2 diff = edgeStart - startPos;
        float t = (diff.x * edgeDir.y - diff.y * edgeDir.x) / denominator;

        // Find the intersection parameter u for the line segment [edgeStart, edgeEnd]
        float u = (diff.x * dir.y - diff.y * dir.x) / denominator;

        // Check if the intersection is within the bounds of both line segments
        if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
            return t;
        }

        return -1.0f; // No valid intersection within the bounds of the segments
    }


    IntersectionResult rayIntersectEdge(const glm::vec2& startPos, const glm::vec2& velocity, const Edge2D& edge) {
        glm::vec2 edgeDir = edge.end - edge.start;
        glm::vec2 dir = glm::normalize(velocity); // Ensure direction is normalized

        // Calculate the perpendicular dot product (cross product in 2D)
        float edgePerpDotDir = edgeDir.x * dir.y - edgeDir.y * dir.x;

        if (std::abs(edgePerpDotDir) < 1e-8) {
            // Lines are parallel or coincident
            return IntersectionResult{false, glm::vec2(), 0.0f};
        }

        glm::vec2 startToEdge = startPos - edge.start;

        // Parameter t for the ray
        float t = (startToEdge.x * edgeDir.y - startToEdge.y * edgeDir.x) / edgePerpDotDir;

        if (t < 0) {
            // Intersection point is behind the start of the ray
            return IntersectionResult{false, glm::vec2(), 0.0f};
        }

        // Parameter u for the edge segment
        float u = (startToEdge.x * dir.y - startToEdge.y * dir.x) / edgePerpDotDir;

        if (u < 0 || u > 1) {
            // Intersection point is outside the bounds of the edge segment
            return IntersectionResult{false, glm::vec2(), 0.0f};
        }

        // Calculate the exact intersection point
        glm::vec2 intersectionPoint = startPos + t * dir;

        return IntersectionResult{true, intersectionPoint, t};
    }




}

