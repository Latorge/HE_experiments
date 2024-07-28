#include "RenderLines.hpp"

namespace RenderLines {
    

    std::vector<DrawSupport::PointInfo> renderSilhouetteLines(HalfedgeDS& halfedgeDS, Scene::Camera& camera) {
        std::vector<DrawSupport::PointInfo> lines;

        DrawSupport::Color linesColor;
        DrawSupport::Color linesColor2;
        linesColor.setHex(0xFFFFFF);  
        linesColor2.setHex(0x006400);  // Dark green  
/*
        std::vector<Face*> frontFaces = updateFrontFaces(halfedgeDS, camera);

        for (auto& he : halfedgeDS.getHalfedges()) {
            if (!he->face || !he->twin || !he->twin->face) continue; // Ensure both faces exist
            if (std::find(frontFaces.begin(), frontFaces.end(), he->face) != frontFaces.end() ^
                std::find(frontFaces.begin(), frontFaces.end(), he->twin->face) != frontFaces.end()) {
                // One face is visible and the other is not
                glm::vec3 start = he->vertex->position;
                glm::vec3 end = he->twin->vertex->position;

                // Push line segment
                lines.push_back({start, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
                lines.push_back({end, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});

            }
       }
*/
        std::unordered_set<Face*> frontFaces = updateFrontFacesSet(halfedgeDS, camera);

        for (auto& he : halfedgeDS.getHalfedges()) {
            if (!he->face || !he->twin || !he->twin->face) continue; // Ensure both faces exist

            // Use unordered_set's find method which is O(1) on average
            bool faceIsFront = frontFaces.find(he->face) != frontFaces.end();
            bool twinFaceIsFront = frontFaces.find(he->twin->face) != frontFaces.end();

            if (faceIsFront ^ twinFaceIsFront) {
                // One face is visible and the other is not, indicating a silhouette edge
                glm::vec3 start = he->vertex->position;
                glm::vec3 end = he->twin->vertex->position;

                // Push line segment
                lines.push_back({start, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
                lines.push_back({end, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});

               // lines.push_back({start, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
               // lines.push_back({camera.position, glm::vec4(linesColor2.r(), linesColor2.g(), linesColor2.b(), 1.0f)});
            }
        }

        return lines;
    }
}