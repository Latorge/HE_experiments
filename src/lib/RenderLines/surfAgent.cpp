#include "SurfSpriteRender.hpp"

namespace RenderLines {


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

    void adjustVelocityDirection(glm::vec3& velocity, const glm::vec3& currentNormal, const glm::vec3& nextNormal) {
        glm::vec3 axis = glm::cross(currentNormal, nextNormal);
        float angle = std::acos(glm::dot(currentNormal, nextNormal) / (glm::length(currentNormal) * glm::length(nextNormal)));

        // Build a quaternion from axis and angle for rotation
        glm::quat rotation = glm::angleAxis(angle, glm::normalize(axis));
        velocity = rotation * velocity;
    }
    

      void SurfAgent::update(float deltaTime) {
            glm::vec3 segmentVector = velocity * deltaTime;
            glm::vec3 newPosition = position + segmentVector;

            // Handle multiple face transitions
            while (deltaTime > 0.0f) {
                bool crossed = false;
                for(Halfedge* edge:currentFace->getHalfedges()) {
                    if (crossesEdge(position, newPosition, edge)) {
                        float timeToCross = timeToIntersectEdge(position, newPosition, edge);
                        glm::vec3 intersectionPoint = position + segmentVector * timeToCross;

                        // Update position to the edge intersection
                        position = intersectionPoint;
                        trail.push_back(position);

                        // Check if there's a twin face
                        if (edge->twin && edge->twin->face) {
                            // Change the face and adjust the direction based on the new face's normal
                            Face* nextFace = edge->twin->face;
                            adjustVelocityDirection(velocity, currentFace->normal, nextFace->normal);
                            currentFace = nextFace;

                            // Recalculate remaining displacement
                            deltaTime *= (1.0f - timeToCross);
                            segmentVector = velocity * deltaTime;
                            newPosition = position + segmentVector;
                        } else {
                            // Handle the case where there is no adjacent face (open boundary)
                            handleOpenBoundary(); // Implement this method according to your needs
                            deltaTime = 0.0f; // Stop further motion in this update
                        }

                        crossed = true;
                        break;  // Break after the first edge crossing
                    }
                }

                if (!crossed) {
                    // No crossing, update the position and exit the loop
                    position = newPosition;
                    trail.push_back(position);
                    break;
                }
            }

            // Ensure the trail does not exceed the maximum length
            if (trail.size() > maxTrailLength) {
                trail.pop_front();  // Remove the oldest element
            }
        }

        void SurfAgent::handleOpenBoundary() {
            // Example: Reflect the velocity vector
            glm::vec3 normal = currentFace->getNormal();// Method to compute the normal of the current face
            velocity = glm::reflect(velocity, normal);

            // Optionally, introduce some randomness or simply stop:
            // velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Stop movement
            // Or random new direction:
            // velocity = generateRandomDirection(); // You would need to implement this
        }


        void SurfAgent::changeFace(Halfedge* edge) {
            if (edge && edge->face) {
                currentFace = edge->face;
                updateBarycentric();
            }
        }

        void SurfAgent::render() const {
            // Placeholder for rendering logic
            for (const auto& pos : trail) {
                std::cout << "Trail Point: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
            }
        }

        glm::vec3 SurfAgent::barycentricToPoint(Face* face, const glm::vec3& bary) const {
            const std::vector<Vertex*> listOfVertex =face->getVertices();
            if(listOfVertex.size()!=3)
                std::cout<<"Surf agent  find face with more then 3 vertex"<<std::endl;

            glm::vec3 p0 = listOfVertex[0]->position;
            glm::vec3 p1 = listOfVertex[1]->position;
            glm::vec3 p2 = listOfVertex[2]->position;

            return glm::vec3(
                p0.x * bary[0] + p1.x * bary[1] + p2.x * bary[2],
                p0.y * bary[0] + p1.y * bary[1] + p2.y * bary[2],
                p0.z * bary[0] + p1.z * bary[1] + p2.z * bary[2]
            );
        }

        void SurfAgent::updateBarycentric() {
            if (currentFace) {
                auto vertices = currentFace->getVertices();
                barycentric = globalToBarycentric(vertices[0]->position, vertices[1]->position, vertices[2]->position, position);
            }
        }

        glm::vec3 SurfAgent::updateBarycentric(Face* face, const glm::vec3& pos) {
            if (face) {
                auto vertices = face->getVertices();
                barycentric = globalToBarycentric(vertices[0]->position, vertices[1]->position, vertices[2]->position, pos);
            }
        }

        glm::vec3 SurfAgent::globalToBarycentric(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p) const {
            glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
            float d00 = glm::dot(v0, v0);
            float d01 = glm::dot(v0, v1);
            float d11 = glm::dot(v1, v1);
            float d20 = glm::dot(v2, v0);
            float d21 = glm::dot(v2, v1);
            float denom = d00 * d11 - d01 * d01;
            float v = (d11 * d20 - d01 * d21) / denom;
            float w = (d00 * d21 - d01 * d20) / denom;
            float u = 1.0f - v - w;
            return glm::vec3(u, v, w);
        }
    


}