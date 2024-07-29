#include "RenderLines.hpp"

namespace RenderLines {
    
    std::vector<DrawSupport::PointInfo> renderSilhouetteLines(HalfedgeDS& halfedgeDS, const std::unordered_set<Face*>& frontFaces, const DrawSupport::Color& linesColor, Scene::Camera& camera) {
        std::vector<DrawSupport::PointInfo> lines;

        float crossGapFactor = 0.0035f; // Placeholder for gap factor adjustment
        std::unordered_set<Halfedge*> processedHalfedges;

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

        for (auto& he : halfedgeDS.getHalfedges()) {

            if (!he->face || !he->twin || !he->twin->face) continue; // Ensure both faces exist

            if (processedHalfedges.find(he) != processedHalfedges.end()) {
                // If halfedge or its twin has been processed, skip it
                continue;
            }

            // Use unordered_set's find method which is O(1) on average
            bool faceIsFront = frontFaces.find(he->face) != frontFaces.end();
            bool twinFaceIsFront = frontFaces.find(he->twin->face) != frontFaces.end();

            if (faceIsFront ^ twinFaceIsFront) {

                // One face is visible and the other is not, indicating a silhouette edge
                glm::vec3 start = he->vertex->position;
                glm::vec3 end = he->twin->vertex->position;

                // Determine which face is facing the camera
                Face* visibleFace = faceIsFront ? he->face : he->twin->face;
                glm::vec3 normal =visibleFace->getNormal();

                // Compute the direction vector and the cross product with the normal
                glm::vec3 dir = end - start;
                glm::vec3 cross = glm::cross(normal, dir);
                cross = glm::normalize(cross) * crossGapFactor; // Apply scaling factor after normalization

                // Offset start and end positions based on cross product for visual effect
                glm::vec3 adjustedStart = start - cross;
                glm::vec3 adjustedEnd = end- cross;

                // Create drawable segments for rendering
                lines.push_back({adjustedStart, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});
                lines.push_back({adjustedEnd,   glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});

                // Mark both this halfedge and its twin as processed
               // processedHalfedges.insert(he);
                processedHalfedges.insert(he->twin);

                // Push line segment
                //lines.push_back({start, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
                //lines.push_back({end, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});

               // lines.push_back({start, glm::vec4(linesColor.red(), linesColor.green(), linesColor.blue(), 1.0f)});
               // lines.push_back({camera.position, glm::vec4(linesColor2.r(), linesColor2.g(), linesColor2.b(), 1.0f)});
            }
        }

        return lines;
    }

    std::vector<DrawSupport::PointInfo> renderSilhouetteLinesWithAngle(HalfedgeDS& halfedgeDS, const std::unordered_set<Face*>& frontFaces, const DrawSupport::Color& linesColor, float angleThresholdDegrees) {
        std::vector<DrawSupport::PointInfo> lines;
        std::unordered_set<Halfedge*> processedHalfedges;
        float radiansThreshold = glm::radians(angleThresholdDegrees);

        for (auto& he : halfedgeDS.getHalfedges()) {
            if (!he->face || !he->twin || !he->twin->face) continue; // Ensure both faces exist
            if (processedHalfedges.find(he) != processedHalfedges.end()) continue; // Skip processed edges

            bool faceIsFront = frontFaces.find(he->face) != frontFaces.end();
            bool twinFaceIsFront = frontFaces.find(he->twin->face) != frontFaces.end();

            if (faceIsFront && twinFaceIsFront) { // Both faces must be visible
                glm::vec3 normal1 = he->face->getNormal();
                glm::vec3 normal2 = he->twin->face->getNormal();
                float angle = std::acos(glm::clamp(glm::dot(glm::normalize(normal1), glm::normalize(normal2)), -1.0f, 1.0f));

                if (angle > radiansThreshold) { // Check if the angle exceeds the threshold
                    glm::vec3 start = he->vertex->position;
                    glm::vec3 end = he->twin->vertex->position;

                    // Add the line segment with adjusted color if the angle condition is met
                    lines.push_back({start, linesColor.vec()});
                    lines.push_back({end, linesColor.vec()});

                    // Mark both this halfedge and its twin as processed
                    processedHalfedges.insert(he);
                    processedHalfedges.insert(he->twin);
                }
            }
        }

        return lines;
    }
}