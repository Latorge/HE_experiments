#include "SurfSpriteRender.hpp"
#include "surfSupportFunctions.hpp"

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
/*
    glm::vec2 projectTo2D(const glm::vec3& pos3D, Face* currentFace) {
        ProjectionAxes axes = getProjectionAxes(currentFace);
        glm::vec3 origin = currentFace->getVertices()[0]->position;
        glm::vec3 relPos = pos3D - origin;
        
        float x = glm::dot(relPos, axes.tangent);
        float y = glm::dot(relPos, axes.bitangent);
        
        return glm::vec2(x, y);
    }
*/
    glm::vec2 projectTo2D(const glm::vec3& pos3D, Face* currentFace) {
        auto vertices = currentFace->getVertices();
        if (vertices.size() < 3) {
            // Not enough vertices to define a plane
            return glm::vec2(0.0f, 0.0f);
        }

        glm::vec3 origin = vertices[0]->position;
        glm::vec3 edge1 = vertices[1]->position - origin;
        glm::vec3 edge2 = vertices[2]->position - origin;

        glm::vec3 localX = glm::normalize(edge1);
        glm::vec3 planeNormal = glm::normalize(glm::cross(edge1, edge2));
        glm::vec3 localY = glm::normalize(glm::cross(planeNormal, localX));

        glm::vec3 relPos = pos3D - origin;
        float x = glm::dot(relPos, localX);
        float y = glm::dot(relPos, localY);


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

    glm::vec3 convert2DVelocityTo3D(const glm::vec2& velocity2D, Face* face) {
        auto vertices = face->getVertices();
        glm::vec3 origin = vertices[0]->position;
        glm::vec3 edge1 = vertices[1]->position - origin;
        glm::vec3 edge2 = vertices[2]->position - origin;

        glm::vec3 localX = glm::normalize(edge1);
        glm::vec3 planeNormal = glm::normalize(glm::cross(edge1, edge2));
        glm::vec3 localY = glm::normalize(glm::cross(planeNormal, localX));

        return localX * velocity2D.x + localY * velocity2D.y;
    }



    glm::vec3 localToBarycentric(const glm::vec2& localPos, Face* face) {
        // Get the vertices of the face
        auto vertices = face->getVertices();
        glm::vec3 v0 = vertices[0]->position;
        glm::vec3 v1 = vertices[1]->position;
        glm::vec3 v2 = vertices[2]->position;

        glm::vec3 v0v1 = v1 - v0;
        glm::vec3 v0v2 = v2 - v0;
        glm::vec3 p = glm::vec3(localPos.x, localPos.y, 0.0f) - v0;

        float d00 = glm::dot(v0v1, v0v1);
        float d01 = glm::dot(v0v1, v0v2);
        float d11 = glm::dot(v0v2, v0v2);
        float d20 = glm::dot(p, v0v1);
        float d21 = glm::dot(p, v0v2);
        float denom = d00 * d11 - d01 * d01;

        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        return glm::vec3(u, v, w);
    }

    void transferVelocityUsingBarycentric(glm::vec2& velocity2D, Face* oldFace, Face* newFace, glm::vec2& localPosition) {
        glm::vec3 barycentric = localToBarycentric(localPosition, oldFace);

        // Assuming new face vertices and calculating new local position
        auto newVertices = newFace->getVertices();
        glm::vec3 newPos3D = newVertices[0]->position * barycentric.x + 
                             newVertices[1]->position * barycentric.y + 
                             newVertices[2]->position * barycentric.z;

        glm::vec2 newPosition2D = projectTo2D(newPos3D, newFace);
        glm::vec2 newVelocityPosition2D = projectTo2D(newPos3D + glm::vec3(velocity2D, 0.0), newFace);

        // Update the velocity based on the new positions
        velocity2D = newVelocityPosition2D - newPosition2D;
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

    glm::vec3 projectPointOntoPlane(const glm::vec3& point, const glm::vec3& planeNormal, const glm::vec3& planePoint) {
        glm::vec3 v = point - planePoint;
        float d = glm::dot(v, planeNormal);
        return point - d * planeNormal;
    }

    glm::vec3 projectVelocityOntoPlane(const glm::vec3& velWorld, Face* face) {
        glm::vec3 normal = glm::normalize(face->getNormal());  // Ensure the normal is normalized
        glm::vec3 velocityProjection = glm::proj(velWorld, normal);  // Project velWorld onto the normal

        return velWorld - velocityProjection;  // Subtract the projection from the original velocity
    }

    glm::vec3 projectPointOntoPlane(const glm::vec3& point, Face* face) {
        // Ensure face is valid
        if (!face) {
            throw std::invalid_argument("Face pointer is null.");
        }

        // Retrieve the normal directly from the Face object
        glm::vec3 normalizedNormal = face->getNormal();

        // Use the first vertex of the face as a point on the plane
        glm::vec3 planePoint = face->getVertices().front()->position;

        // Vector from point on the plane to the point in space
        glm::vec3 pointVector = point - planePoint;

        // Project the point vector onto the plane normal
        glm::vec3 pointProjection = glm::proj(pointVector, normalizedNormal);

        // Subtract the projection from the point to get the closest point on the plane
        glm::vec3 closestPointOnPlane = point - pointProjection;

        return closestPointOnPlane;
    }



    glm::vec3 calculateNormal(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
        return glm::normalize(glm::cross(v2 - v1, v3 - v1));
    }

    glm::quat rotationBetweenTriangles(const glm::vec3& fromNormal, const glm::vec3& toNormal) {
        glm::vec3 axis = glm::cross(fromNormal, toNormal);
        float lengthSq = glm::length2(axis);

        if (lengthSq < 1e-9) {
            return glm::quat(1.0, 0.0, 0.0, 0.0); // Return identity quaternion
        }

        // Normalize the axis
        axis = glm::normalize(axis);
       // float angle = acos(glm::clamp(glm::dot(fromNormal, toNormal), -1.0f, 1.0f));
        float angle = glm::angle(fromNormal, toNormal);
        return glm::angleAxis(angle, axis);
    }


    glm::quat calculateRotationBetweenFaces(Face* fromFace, Face* toFace) {
        glm::vec3 fromNormal = glm::normalize(glm::cross(fromFace->getVertices()[1]->position - fromFace->getVertices()[0]->position, fromFace->getVertices()[2]->position - fromFace->getVertices()[0]->position));
        glm::vec3 toNormal = glm::normalize(glm::cross(toFace->getVertices()[1]->position - toFace->getVertices()[0]->position, toFace->getVertices()[2]->position - toFace->getVertices()[0]->position));

        glm::vec3 axis = glm::cross(fromNormal, toNormal);
        if (glm::length2(axis) < 1e-9) {
            return glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Faces are parallel
        }

        axis = glm::normalize(axis);
        float angle = acos(glm::clamp(glm::dot(fromNormal, toNormal), -1.0f, 1.0f));
        return glm::angleAxis(angle, axis);
    }

    float calculateVelocityAngle(const glm::vec2& velocity, Halfedge* edge) {
        glm::vec3 edge1 = edge->vertex->position-edge->twin->vertex->position;
        glm::vec3 localX = glm::normalize(edge1); // Local x-axis
        glm::vec2 localVelocity = glm::vec2(glm::dot(velocity, glm::vec2(localX.x, localX.y)), glm::dot(velocity, glm::vec2(-localX.y, localX.x)));

        return atan2(localVelocity.y, localVelocity.x);
    }

    glm::vec2 constructVelocityFromAngle(float angle, float magnitude, Face* face, Halfedge* edge) {
        glm::vec3 edge1 =   edge->vertex->position-edge->twin->vertex->position;
        glm::vec3 localX = glm::normalize(edge1); // Local x-axis
        glm::vec3 localY = glm::normalize(glm::cross(glm::cross(edge1, face->getVertices()[2]->position - face->getVertices()[0]->position), edge1)); // Local y-axis orthogonal to x

        glm::vec2 newVelocity = glm::vec2(cos(angle), sin(angle)) * magnitude; // Velocity vector in local face coordinates
        return glm::vec2(glm::dot(newVelocity, glm::vec2(localX.x, localX.y)), glm::dot(newVelocity, glm::vec2(localY.x, localY.y)));
    }

    glm::vec3 calculate3DVelocity(const glm::vec2& velocity2D, const glm::vec2& position2D, Face* face) {
        glm::vec3 point3D = projectTo3D(position2D, face); // Project the current 2D position to 3D
        glm::vec3 point3DWithVelocity = projectTo3D(position2D + velocity2D, face); // Project the 2D position plus velocity to 3D

        return point3DWithVelocity - point3D; // The 3D velocity vector
    }

    glm::vec3 rotateVector(const glm::vec3& vec, const glm::vec3& axis, float angle) {
        // Create a rotation matrix around the axis by the given angle
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

        // Transform the vector by the rotation matrix
        glm::vec4 rotatedVec = rotMat * glm::vec4(vec, 1.0f);

        // Convert back to vec3 and return
        return glm::vec3(rotatedVec);
    }

    float angleBetweenFaces(Face* face1, Face* face2) {
        // Assume Face class has a method getNormal() that returns glm::vec3
        glm::vec3 normal1 = glm::normalize(face1->getNormal());  // Normalize to be safe
        glm::vec3 normal2 = glm::normalize(face2->getNormal());

        glm::vec3 axis = glm::cross(normal1, normal2);
        if (glm::length2(axis) < 1e-9) {
            return 0.0f; // Faces are parallel
        }

        // Calculate the angle between the normals
        float angleRadians = glm::angle(normal1, normal2);

        // Convert radians to degrees
        float angleDegrees = glm::degrees(angleRadians);

        return angleDegrees;
    }

    bool isApproachingVertex(const glm::vec2& pos, Face* face) {
        glm::vec3 pos3D = projectTo3D(pos, face);
        for (auto& v : face->getVertices()) {
            if (glm::distance(pos3D, v->position) < 0.01) {  // Threshold depends on model scale
                return true;
            }
        }
        return false;
    }

    glm::vec3 safeNormalize(const glm::vec3& v) {
        float len = glm::length(v);
        if (len < 1e-5)  // Adjust threshold based on expected vector magnitudes in your application
            return glm::vec3(1, 0, 0); // Return a default normal if too small
        return v / len;
    }


}

