#include "SurfSpriteRender.hpp"

namespace RenderLines {

    SurfAgent::SurfAgent(glm::vec3 startPosition,
         Face* startFace,
         glm::vec2 initialVelocity,
         size_t maxTrailLength)
        : posWorld(startPosition), currentFace(startFace), velLocal(initialVelocity), maxTrailLength(maxTrailLength)
    {
        initialize2DEdges(currentFace);
        updateWorldPosToLocalPos();
    }

    void SurfAgent::updateWorldPosToLocalPos()
    {
        posLocal = projectTo2D(posWorld, currentFace);
       // velLocal = projectTo2D(velWorld, currentFace);
    }


    void SurfAgent::initialize2DEdges(Face* face) {
        edges2D.clear();
        auto vertices = face->getVertices();
        glm::vec2 prevVertex3D = projectTo2D(vertices.back()->position,face);

        for (auto& vertex : vertices) {
            glm::vec2 vertex3D = projectTo2D(vertex->position, face);
            Edge2D edge;
            edge.start = prevVertex3D;
            edge.end =vertex3D;
            edge.halfedge = vertex->halfedge->prev;
            prevVertex3D = vertex3D;

            edges2D.push_back(edge);
        }
    }


    std::tuple<Edge2D, bool, glm::vec2> SurfAgent::crossesEdges(const glm::vec2& startPos, const glm::vec2& endPos) {
        glm::vec2 dir = endPos - startPos;

        for (auto& edge : edges2D) {
            glm::vec2 edgeStart = edge.start;
            glm::vec2 edgeEnd = edge.end;

            glm::vec2 edgeDir = edgeEnd - edgeStart;
            glm::vec2 edgeNormal(-edgeDir.y, edgeDir.x);

            float denom = glm::dot(dir, edgeNormal);
            if (std::abs(denom) < 1e-8) {
                continue; // Edges are parallel
            }

            float num = glm::dot(edgeStart - startPos, edgeNormal);
            float t = num / denom;

            if (t >= 0.0f && t <= 1.0f) {
                glm::vec2 intersection = startPos + t * dir;
                if (pointOnEdge(intersection, edgeStart, edgeEnd)) {
                    return std::make_tuple(edge, true, intersection);
                }
            }
        }
        return std::make_tuple(Edge2D(), false, glm::vec2());
    }

    bool SurfAgent::pointOnEdge(const glm::vec2& point, const glm::vec2& edgeStart, const glm::vec2& edgeEnd) {
        // Check if a point is on a line segment using the parametric equation
        glm::vec2 edgeDir = edgeEnd - edgeStart;
        glm::vec2 toPoint = point - edgeStart;

        if (std::abs(edgeDir.x) > std::abs(edgeDir.y)) {
            // Compare x-coordinates
            float t = toPoint.x / edgeDir.x;
            return t >= 0.0f && t <= 1.0f && std::abs(edgeStart.y + t * edgeDir.y - point.y) < 1e-8;
        } else {
            // Compare y-coordinates
            float t = toPoint.y / edgeDir.y;
            return t >= 0.0f && t <= 1.0f && std::abs(edgeStart.x + t * edgeDir.x - point.x) < 1e-8;
        }
    }

    std::tuple<Edge2D, bool, glm::vec2> SurfAgent::crossesEdgesRay(const glm::vec2& startPos, const glm::vec2& endPos) {
        for (auto& edge : edges2D)
        {
            auto intersection = rayIntersectEdge(startPos, velLocal, edge);
            if (intersection.intersect) {
                //float timeToImpact = intersection.distance/ glm::length(velLocal);
                //if(timeToImpact <= deltaTime)
                {
                    //handleEdgeCrossing(edge, timeToImpact);
                    return std::make_tuple(edge, true, intersection.point);
                }
            }
        }
        return std::make_tuple(Edge2D(), false, glm::vec2());
    }


   void SurfAgent::update(float _deltaTime) {
        deltaTime = _deltaTime;
        // Convert velocity and position into 2D using currentFace plane projection
        
        glm::vec2 segmentVector2D = velLocal * deltaTime;
        glm::vec2 newPosition2D = posLocal + segmentVector2D;

        Edge2D* lastCrossedEdge = nullptr;

        // Handle multiple face transitions
        while (deltaTime > 0.0f) {
            auto [crossedEdgeN, crossedN, intersectionPoint2DN]=crossesEdgesRay(posLocal, newPosition2D);
            if(crossedEdgeN.halfedge)
                nextFace=crossedEdgeN.halfedge->face;
            else
                nextFace=currentFace->halfedge->twin->face;
            auto [crossedEdge, crossed, intersectionPoint2D] = crossesEdges(posLocal, newPosition2D);
            if (crossed) {
                if (lastCrossedEdge && lastCrossedEdge->halfedge == crossedEdge.halfedge) {
                    break;  // Prevent immediate back-and-forth transition
                }
                
                // Calculate the time used to reach the crossing point
                float timeUsed = glm::length(intersectionPoint2D - posLocal) / glm::length(segmentVector2D);

                // Ensure posLocal is updated before calculating the remaining deltaTime
                posLocal = intersectionPoint2D;
                glm::vec3 intersectionPoint3D = projectTo3D(posLocal, currentFace);
                trail.push_back(intersectionPoint3D);

                deltaTime *= (1.0f - timeUsed);  // Adjust deltaTime for the remaining part of the move

                if (crossedEdge.halfedge->twin && crossedEdge.halfedge->twin->face) {
                    Face* nextFaceTemp = crossedEdge.halfedge->twin->face;
                    adjustVelocityDirection(velLocal, currentFace, currentFace->getNormal(), nextFaceTemp->getNormal());
                    currentFace =nextFaceTemp;

                    // Recalculate new position in 2D for the new face
                    segmentVector2D = velLocal * deltaTime;
                    newPosition2D = projectTo2D(intersectionPoint3D, currentFace) + segmentVector2D;

                    initialize2DEdges(currentFace);
                    lastCrossedEdge = &crossedEdge;

                } else {
                    handleOpenBoundary(crossedEdge.start, crossedEdge.end);
                    break;  // Stop further motion if at an open boundary
                }

                if (timeUsed == 0) {
                    break;
                }
            } else {
                posLocal = newPosition2D;
                glm::vec3 newPosition3D = projectTo3D(posLocal, currentFace);
                trail.push_back(newPosition3D);
                break;  // No more edges to cross
            }

            // Safety break condition to prevent potential infinite loop
            if (deltaTime < 1e-6f) {
                break;
            }
        }

        // Ensure the trail does not exceed the maximum length
        if (trail.size() > maxTrailLength) {
            trail.pop_front();  // Remove the oldest element
        }
    }



    void SurfAgent::adjustVelocityDirection(glm::vec2& velocity2D, Face* face, const glm::vec3& oldNormal, const glm::vec3& newNormal) {
        if (glm::length(oldNormal - newNormal) < 1e-8) {
            // Normals are the same or very close to each other, no adjustment needed
            return;
        }

        // Find the rotation axis and angle from the old normal to the new normal
        glm::vec3 rotationAxis = glm::cross(oldNormal, newNormal);
        if (glm::length(rotationAxis) < 1e-8) {
            // Normals are opposite or very close to opposite, handle special case
            rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f); // Arbitrary orthogonal axis
        } else {
            rotationAxis = glm::normalize(rotationAxis);
        }

        float dotProduct = glm::dot(oldNormal, newNormal);
        float clampedDotProduct = glm::clamp(dotProduct / (glm::length(oldNormal) * glm::length(newNormal)), -1.0f, 1.0f);
        float angle = glm::acos(clampedDotProduct);

        // Create a quaternion from axis and angle
        glm::quat rotation = glm::angleAxis(angle, rotationAxis);

        // Now, apply the rotation to the velocity vector. First, lift the 2D velocity back to 3D.
        glm::vec3 velocity3D(velocity2D.x, velocity2D.y, 0.0f);

        // Transform the 3D velocity vector by the rotation
        velocity3D = rotation * velocity3D;

        velocity2D=projectTo2D(velocity3D, face);

    }



    void SurfAgent::handleOpenBoundary3D() {
        // Example: Reflect the velocity vector
        glm::vec3 normal = currentFace->getNormal();// Method to compute the normal of the current face
        velWorld = glm::reflect(velWorld, normal);

        // Optionally, introduce some randomness or simply stop:
        // velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Stop movement
        // Or random new direction:
        // velocity = generateRandomDirection(); // You would need to implement this
    }

    void SurfAgent::handleOpenBoundary(const glm::vec2& edgeStart, const glm::vec2& edgeEnd) {
            glm::vec2 edgeVector = edgeEnd - edgeStart;

        // Calculate the normal to this edge in the local coordinate system
        glm::vec2 edgeNormal(-edgeVector.y, edgeVector.x); // Rotate by 90 degrees to get the normal
        edgeNormal = glm::normalize(edgeNormal);

        // Reflect velLocal around edgeNormal
        velLocal = glm::reflect(velLocal, edgeNormal);

        // Normalize the result to maintain consistent speed or adjust speed as necessary
        velLocal = glm::normalize(velLocal) * glm::length(velLocal);

        // Optionally, apply randomness or stop the movement
        // Random new direction:
        // velLocal = sphericalRand(1.0f); // Requires adaptation or implementation for 2D
        // Stop movement:
        // velLocal = glm::vec2(0.0f, 0.0f);
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

        return p0*bary.x+p1*bary.y+p2*bary.z;
    }

    
    void SurfAgent::updateBarycentric() {
        if (currentFace) {
            auto vertices = currentFace->getVertices();
            barycentric = globalToBarycentric(vertices[0]->position, vertices[1]->position, vertices[2]->position, posWorld);
        }
    }

    glm::vec3 SurfAgent::updateBarycentric(Face* face, const glm::vec3& pos) {
        if (face) {
            auto vertices = face->getVertices();
            barycentric = globalToBarycentric(vertices[0]->position, vertices[1]->position, vertices[2]->position, pos);
        }
    }

    glm::vec3 SurfAgent::globalToBarycentric(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p) const {
            // 1. Calculate the plane normal
        glm::vec3 n = glm::cross(b - a, c - a);

        // 2. Project p onto the plane
        glm::vec3 ap = p - a;
        glm::vec3 projection = p - (glm::dot(ap, n) / glm::dot(n, n)) * n;

        // 3. Calculate barycentric coordinates of the projection
        glm::vec3 v0 = b - a, v1 = c - a, v2 = projection - a;
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




/*
      void SurfAgent::update3d(float deltaTime) {
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
*/

    


}