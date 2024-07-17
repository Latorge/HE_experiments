// Operations.hpp
#ifndef DRAWSUPPORT_HPP
#define DRAWSUPPORT_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

#include "../Operations/Operations.hpp"

namespace DrawSupport {

    struct PointInfo {
        glm::vec3 position;
        glm::vec3 color;
    };

    struct GeometryData {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec4> colors;
        std::vector<glm::vec3> normals;
    };

    struct Color {
        glm::vec4 color;  // Using glm::vec4 to store RGBA values directly

        // Sets the color using a hexadecimal value
        void setHex(unsigned int hexValue) {
            color.r = ((hexValue >> 16) & 0xFF) / 255.0f;
            color.g = ((hexValue >> 8) & 0xFF) / 255.0f;
            color.b = (hexValue & 0xFF) / 255.0f;
            color.a = 1.0f;  // Default to fully opaque unless specified otherwise
        }

        // Optionally, you might want to add a constructor for convenience
        Color(unsigned int hexValue = 0xFFFFFF) { // Default white color
            setHex(hexValue);
        }

        // Getters for individual components if needed (not mandatory as glm::vec4 is accessible)
        float red() const { return color.r; }
        float green() const { return color.g; }
        float blue() const { return color.b; }
        float alpha() const { return color.a; }

        // Direct access to glm::vec4 for operations that benefit from vector operations
        glm::vec4& vec() { return color; }
        const glm::vec4& vec() const { return color; }
    };


    std::vector<DrawSupport::PointInfo> setHalgEdgesLines(HalfedgeDS& halfedgeDS);

    std::vector<DrawSupport::PointInfo> setHalgEdgesLines2(HalfedgeDS& halfedgeDS, bool onlyBoundaryLines=false);
    std::vector<DrawSupport::PointInfo> setHalgEdgesLines3(HalfedgeDS& halfedgeDS);

    std::vector<DrawSupport::PointInfo> setHalfEdgesLines4(HalfedgeDS &halfedgeDS, bool onlyBoundaryLines);

    //Tringulate 

    glm::vec3 calculateCentralPoint(const std::vector<Vertex *> &vertices);
    void addTriangleToData(DrawSupport::GeometryData &data, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &normal);
    DrawSupport::GeometryData triangulateFace(Face *face);
    DrawSupport::GeometryData processTriangulateHalfedgeDS(HalfedgeDS &halfedgeDS);
}

#endif // DRAWSUPPORT_HPP
