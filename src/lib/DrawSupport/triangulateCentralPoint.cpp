// Operations.hpp
#include "drawSupport.hpp"

namespace DrawSupport {
    

    glm::vec3 calculateCentralPoint(const std::vector<Vertex*>& vertices) {
        glm::vec3 centroid(0.0f);
        for (auto& v : vertices) {
            centroid += v->position;
        }
        centroid /= static_cast<float>(vertices.size());
        return centroid; // Return the calculated centroid directly
    }

    void addTriangleToData(DrawSupport::GeometryData& data, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& normal) {
        data.vertices.push_back(p1);
        data.vertices.push_back(p2);
        data.vertices.push_back(p3);

        // Add the same color for all vertices (change as needed)
        glm::vec4 color = glm::vec4(0.75, 0.75, 0.75, 1.0); // White color
        data.colors.push_back(color);
        data.colors.push_back(color);
        data.colors.push_back(color);

        // Add the normal for all vertices
        data.normals.push_back(normal);
        data.normals.push_back(normal);
        data.normals.push_back(normal);
    }

    DrawSupport::GeometryData triangulateFace(Face* face) {
        DrawSupport::GeometryData data;

        // First, gather all vertices of the face
        std::vector<Vertex*> vertices = face->getVertices();

        // Check the number of vertices
        if (vertices.size() < 3) {
            // Not enough vertices to form a face
            return data;
        }

        glm::vec3 normal = face->getNormal(); // Calculate the normal for the face

        if (vertices.size() == 3) {
            // Already a triangle, directly use the vertices
            for (int i = 0; i < 3; i++) {
                data.vertices.push_back(vertices[i]->position);
                data.colors.push_back(glm::vec4(0.75, 0.75, 0.75, 1.0)); // Assuming white color for simplicity
                data.normals.push_back(normal);
            }
        } else if (vertices.size() == 4) {
            // Quadrilateral, split into two triangles
            addTriangleToData(data, vertices[0]->position, vertices[1]->position, vertices[2]->position, normal);
            addTriangleToData(data, vertices[0]->position, vertices[2]->position, vertices[3]->position, normal);
        } else {
            // More complex polygon, use a fan triangulation
            glm::vec3 centralPoint = calculateCentralPoint(vertices);
            for (int i = 1; i < vertices.size() - 1; ++i) {
                addTriangleToData(data, centralPoint, vertices[i]->position, vertices[i + 1]->position, normal);
            }
            addTriangleToData(data, centralPoint, vertices.back()->position, vertices[0]->position, normal);
        }

        return data;
    }


    DrawSupport::GeometryData processTriangulateHalfedgeDS(HalfedgeDS& halfedgeDS) {
        DrawSupport::GeometryData data;
        for (Face* face : halfedgeDS.getFaces()) {
            DrawSupport::GeometryData faceData = triangulateFace(face);
            data.vertices.insert(data.vertices.end(), faceData.vertices.begin(), faceData.vertices.end());
            data.colors.insert(data.colors.end(), faceData.colors.begin(), faceData.colors.end());
            data.normals.insert(data.normals.end(), faceData.normals.begin(), faceData.normals.end());
        }
        return data;
}








}

