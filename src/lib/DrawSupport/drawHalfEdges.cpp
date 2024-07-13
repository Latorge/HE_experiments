// Operations.hpp
#include "drawSupport.hpp"

namespace DrawSupport {

    std::vector<DrawSupport::PointInfo> setHalgEdgesLines(HalfedgeDS& halfedgeDS) {
        std::vector<DrawSupport::PointInfo> vertices;
        float arrowSize = 0.035f;
        Color linesColor;
        linesColor.setHex(0xFFFFFF);  // White

        for (auto& he : halfedgeDS.getHalfedges()) {
            glm::vec3 start = he->vertex->position;
            glm::vec3 end = he->twin->vertex->position;
            glm::vec3 dir = glm::normalize(end - start);
            glm::vec3 normal = glm::normalize(he->face ? he->face->normal : glm::vec3(1.0f));  // Use a default normal if face is nullptr

            // Push the line segment
            vertices.push_back({start, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
            vertices.push_back({end, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});

            // Calculate arrow heads
            glm::vec3 arrowHeadLDir = glm::normalize(glm::cross(normal, dir) - dir) * arrowSize;
            glm::vec3 arrowHeadRDir = glm::normalize(-glm::cross(normal, dir) - dir) * arrowSize;

            glm::vec3 arrowLeft = end + arrowHeadLDir;
            glm::vec3 arrowRight = end + arrowHeadRDir;

            // Arrow left
            vertices.push_back({end, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
            vertices.push_back({arrowLeft, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});

            // Arrow right
            vertices.push_back({end, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
            vertices.push_back({arrowRight, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
        }

        return vertices;
    }


    std::vector<DrawSupport::PointInfo> setHalgEdgesLines2(HalfedgeDS& halfedgeDS) {
        const float arrowSize = 0.015f;
        const float crossGapFactor = 0.03f;
        const float dirGapFactor = 0.15f;
        const float normalGapFactor = 0.01f;

        Color linesColor;
        linesColor.setHex(0xFFFFFF);  // White color for lines

        std::vector<DrawSupport::PointInfo> vertices;

        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_int_distribution<> distr(0, 0xFFFFFF);

        for (auto& face : halfedgeDS.getFaces()) {
            face->calculateNormal();
        }

        auto loops = halfedgeDS.loops();
        for (auto& loop : loops) {
            Color randomColor;
            randomColor.setHex(distr(eng));

            for (auto& he : loop) {
                glm::vec3 start = he->vertex->position;
                glm::vec3 end = he->twin->vertex->position;
                glm::vec3 normal = he->face ? he->face->normal : glm::vec3(1, 0, 0);

                glm::vec3 dir = glm::normalize(end - start);
                glm::vec3 cross = glm::cross(normal, dir);

                dir *= dirGapFactor;
                normal *= normalGapFactor;
                cross *= crossGapFactor;

                glm::vec3 arrowHeadLDir = glm::normalize(cross - dir) * arrowSize;
                glm::vec3 arrowHeadRDir = glm::normalize(-cross - dir) * arrowSize;

                // Add line start and end points
                vertices.push_back({start, linesColor.color});
                vertices.push_back({end, linesColor.color});

                // Add arrowheads
                vertices.push_back({end, linesColor.color});
                vertices.push_back({end + arrowHeadLDir, randomColor.color});
                vertices.push_back({end, linesColor.color});
                vertices.push_back({end + arrowHeadRDir, randomColor.color});
            }
        }

        return vertices;
    }

}

