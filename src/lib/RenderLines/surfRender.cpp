#include "SurfSpriteRender.hpp"

namespace RenderLines {


    SurfRender::SurfRender(HalfedgeDS* halfedgeDS, int initNumberAgents)
    : halfedgeDS(halfedgeDS) {
        initializeAgents(initNumberAgents);
    }

    void SurfRender::initializeAgents(int count) {
        for (int i = 0; i < count; ++i) {
            // Assuming the HalfedgeDS has a method to get a random face
            Face* startFace = halfedgeDS->getRandomFace();
            glm::vec3 startPosition = startFace->calculateCenterPoint(); // Start at the center of the face
            glm::vec3 initialVelocity = sphericalRand(1.0f); // Random velocity direction

            agents.emplace_back(startPosition, startFace, initialVelocity, 100);
        }
    }

    void SurfRender::update(float deltaTime) {
        for (auto& agent : agents) {
            agent.update(deltaTime);
            // Handle boundary conditions
           // if (agent.isAtBoundary()) {
           //     agent.handleOpenBoundary();
           // }
        }
    }

    void SurfRender::render() {
        for (const auto& agent : agents) {
            agent.render();
        }
    }

    std::vector<DrawSupport::PointInfo> SurfRender::collectTrailSegments() {
        std::vector<DrawSupport::PointInfo> lines;
        DrawSupport::Color linesColor;
         linesColor.setRGB(1.0f, 1.0f, 1.0f, 1.0f);  // White color for illustration

        for (auto& agent : agents) {
            const auto& trail = agent.getTrail();
            if (trail.empty()) continue;  // Skip if no points in the trail

            glm::vec3 lastPoint = trail.front();  // Start from the first point
            glm::vec3 currentPoint;

            auto it = trail.begin();
            size_t count = 1; // Start counting from the first point

            // Draw the actual segments from the trail
            while (it != trail.end()) {
                currentPoint = *it++;
                lines.push_back({lastPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});
                lines.push_back({currentPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});
                lastPoint = currentPoint;
                count++;
            }

            // If the trail length is less than maxTrailLength, extend it to the last known point
            while (count < agent.getMaxTrailLength()) {
                lines.push_back({lastPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});
                lines.push_back({lastPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});  // Repeat the last point
                count++;
            }
        }

        return lines;
    }


     // Example function to check if an agent is at a boundary
    bool SurfRender::isAtBoundary(const SurfAgent& agent) {
        // Placeholder for boundary check logic
        return false; // Change based on actual boundary detection logic
    }
    


}