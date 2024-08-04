// Operations.hpp
#ifndef TRIANGLEFRAME_HPP
#define TRIANGLEFRAME_HPP

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

#include <unordered_set>


namespace RenderLines {

   class TriangleFrame {
        public:
        glm::vec3 a, b, c; // Vertices of the triangle
        glm::vec3 normal;
        glm::mat4 transform;
        glm::mat4 invTransform;
        std::vector<glm::vec3> vertices;
        Face* face;

        // Default constructor
        TriangleFrame() : a(0.0f), b(0.0f), c(0.0f), normal(0.0f), transform(1.0f), invTransform(1.0f), vertices{a, b, c} {}

        // Constructor from a Face object
        TriangleFrame(Face* _face) {
            face=_face;
            if (face && face->getVertices().size() >= 3) {
                const std::vector<Vertex*> verts = face->getVertices();
                a = verts[0]->position;
                b = verts[1]->position;
                c = verts[2]->position;
                vertices = {a, b, c};
                normal = face->getNormal(); // Assuming getNormal() returns a normalized vector

                update(); // Setup transformation matrices
            }
        }

        void update() {
            glm::vec3 _vec0 = glm::normalize(b - a);
            glm::vec3 _vec1 = glm::normalize(glm::cross(_vec0, normal));

            transform = glm::mat4(1.0f); // Reset transform
            transform = glm::translate(transform, a); // Translate to vertex a
            transform[0] = glm::vec4(_vec0, 0.0f); // Set X basis vector
            transform[1] = glm::vec4(_vec1, 0.0f); // Set Y basis vector
            transform[2] = glm::vec4(normal, 0.0f); // Set Z basis vector
            invTransform = glm::inverse(transform);
        }

        glm::vec3 projectPoint(glm::vec3 target) {
            target = glm::vec3(invTransform * glm::vec4(target, 1.0f));
            target.z = 0;
            return glm::vec3(transform * glm::vec4(target, 1.0f));
        }

        glm::vec3 projectDirection(glm::vec3 target) {
            target = glm::mat3(invTransform) * target;
            target.z = 0;
            return glm::mat3(transform) * target;
        }

        int intersectEdge(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& target) {
            float dist = std::numeric_limits<float>::infinity();
            int index = -1;

            for (int i = 0; i < 3; ++i) {
                int i0 = i;
                int i1 = (i + 1) % 3;

                glm::vec3 v0 = vertices[i0];
                glm::vec3 v1 = vertices[i1];
                glm::vec3 edgeNormal = glm::normalize(glm::cross(v1 - v0, normal));

                float d = glm::dot(rayOrigin - v0, edgeNormal);
                if (d > 0) continue; // Ray does not intersect the plane

                float t = -d / glm::dot(rayDirection, edgeNormal);
                if (t >= 0 && t < dist) {
                    dist = t;
                    index = i;
                }
            }

            target = rayOrigin + rayDirection * dist;
            return index;
        }

        Halfedge* intersectHalfEdge(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& target) {
            float dist = std::numeric_limits<float>::infinity();
            const float EPSILON = 1e-6f;
            const float EPSILON_LENGHT = 1e-4f;
            Halfedge* intersectedEdge = nullptr;

            std::vector<Halfedge*> halfEdges = face->getHalfedges();
            glm::vec3 normal = face->getNormal();

            for (Halfedge* he : halfEdges) {
                glm::vec3 v0 = he->vertex->position;
                glm::vec3 v1 = he->next->vertex->position;  // Using next vertex in the half-edge loop
                glm::vec3 edgeNormal = glm::normalize(glm::cross(v1 - v0, normal));

                float d = glm::dot(rayOrigin - v0, edgeNormal);
                if (d > 0) continue;  // Ray does not intersect the plane, or is not directed towards it

                float t = -d / glm::dot(rayDirection, edgeNormal);
                if (t >= 0 && t < dist) {
                    dist = t;
                    intersectedEdge = he;
                }
                
/*
                float d = glm::dot(rayOrigin - v0, edgeNormal);

                if (fabs(d) < EPSILON) d = 0.0f;  // Adjust d when it's close to zero

                float t = -d / glm::dot(rayDirection, edgeNormal);
                // Check if ray potentially intersects the plane within the bounds of the edge
                if (t >= 0 && t < dist) {
                    glm::vec3 intersectionPoint = rayOrigin + float(t)* rayDirection;
                    glm::vec3 edgeVector = v1 - v0;
                    glm::vec3 toIntersection = intersectionPoint - v0;
                    
                    // Project the intersection point onto the edge vector
                    float projLength = glm::dot(toIntersection, glm::normalize(edgeVector));
                    float edgeLength = glm::length(edgeVector);
                    
                    // Check if the projected length is within the bounds of the edge vector
                    if (projLength >=-EPSILON_LENGHT && projLength <= edgeLength+EPSILON_LENGHT) {
                        dist = t;
                        intersectedEdge = he;
                        target = intersectionPoint;  // Update the target with the current intersection point
                    }
                }
*/
            }

            if (intersectedEdge) {
                target = rayOrigin + rayDirection * dist;  // Calculate intersection point
            }
            if(!intersectedEdge)
                std::cout<<"no cross with HE found"<<std::endl;

            return intersectedEdge;
        }

        void copy(const TriangleFrame& source) {
            a = source.a;
            b = source.b;
            c = source.c;
            normal = source.normal;
            transform = source.transform;
            invTransform = source.invTransform;
            vertices = source.vertices;
        }
    };

    
}

#endif // TRIANGLEFRAME_HPP
