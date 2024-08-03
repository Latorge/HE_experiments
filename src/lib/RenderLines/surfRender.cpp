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
            glm::vec2 initialVelocity = circularRand(0.15f); // Random velocity direction

            agents.emplace_back(startPosition, startFace, initialVelocity, 1000);
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

        DrawSupport::Color faceColor;
        faceColor.setRGB(0.5f, 1.0f, 0.5f, 1.0f);  // Light green color for faces

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

             // Draw the current face of the agent as a triangle
            if (agent.getCurrentFace()) {
                const auto& vertices = agent.getCurrentFace()->getVertices();
                for (size_t i = 0; i < vertices.size(); ++i) {
                    size_t next = (i + 1) % vertices.size();
                    glm::vec3 start = vertices[i]->position;
                    glm::vec3 end = vertices[next]->position;
                    lines.push_back({start, glm::vec4(faceColor.r(), faceColor.g(), faceColor.b(), 1.0f)});
                    lines.push_back({end, glm::vec4(faceColor.r(), faceColor.g(), faceColor.b(), 1.0f)});
                }
            }
        }

        return lines;
    }


     // Example function to check if an agent is at a boundary
    bool SurfRender::isAtBoundary(const SurfAgent& agent) {
        // Placeholder for boundary check logic
        return false; // Change based on actual boundary detection logic
    }

    std::vector<DrawSupport::PointInfo> SurfRender::drawProjectedAgentsAndFaces(float planeZ) {
        std::vector<DrawSupport::PointInfo> points;
        DrawSupport::Color agentColor;
        agentColor.setRGB(0.0f, 1.0f, 0.0f, 1.0f);  
        DrawSupport::Color faceColor;
        faceColor.setRGB(0.5f, 0.5f, 1.0f, 1.0f); // Light blue for faces
        DrawSupport::Color nextFaceColor;
        nextFaceColor.setRGB(0.75f, 0.75f, 1.0f, 1.0f); // Light blue for faces

        for (auto& agent : agents) {
            // Convert agent position from 2D back to 3D

            const auto& trail = agent.getTrail();
            if (trail.empty()) continue;  // Skip if no points in the trail

            // Calculate the centroid of the triangle
            auto vertices = agent.getCurrentFace()->getVertices();

            glm::vec3 lastPoint = trail.front();  // Start from the first point
            glm::vec3 currentPoint;

            auto it = trail.begin();
            size_t count = 1; // Start counting from the first point
/*
            // Draw the actual segments from the trail
            while (it != trail.end()) {
                currentPoint = *it++;
                //lines.push_back({lastPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});
                glm::vec3 agentPos3D1= convertTo3D(projectTo2D(lastPoint, agent.getCurrentFace()), planeZ);
                glm::vec3 agentPos3D2= convertTo3D(projectTo2D(currentPoint, agent.getCurrentFace()), planeZ);
                points.push_back({agentPos3D1, glm::vec4(agentColor.r(), agentColor.g(), agentColor.b(), agentColor.a())});
                points.push_back({agentPos3D2, glm::vec4(agentColor.r(), agentColor.g(), agentColor.b(), agentColor.a())});
                //lines.push_back({currentPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});

                lastPoint = currentPoint;
                count++;
            }
*/
/*
            const auto& trailFace = agent.getFaceTrail();
            for(int i=0; i<trail.size();i++)
            {
                currentPoint =trail[i];                //lines.push_back({lastPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});
                glm::vec3 agentPos3D1= convertTo3D(projectTo2D(lastPoint, trailFace[i].second), planeZ);
                glm::vec3 agentPos3D2= convertTo3D(projectTo2D(currentPoint, trailFace[i].second), planeZ);
                points.push_back({agentPos3D1, glm::vec4(agentColor.r(), agentColor.g(), agentColor.b(), agentColor.a())});
                points.push_back({agentPos3D2, glm::vec4(agentColor.r(), agentColor.g(), agentColor.b(), agentColor.a())});
                //lines.push_back({currentPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});

                lastPoint = currentPoint;
                count++;
            }
*/
/*
             const auto& trailFace = agent.getFaceTrail();
            for(int i=1; i<trail.size();i++)
            {
                currentPoint =trail[i];                //lines.push_back({lastPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});
                glm::vec3 agentPos3D1= convertTo3D(trailFace[i].first, planeZ);
                glm::vec3 agentPos3D2= convertTo3D(trailFace[(i+1)%trail.size()].first, planeZ);
                points.push_back({agentPos3D1, glm::vec4(agentColor.r(), agentColor.g(), agentColor.b(), agentColor.a())});
                points.push_back({agentPos3D2, glm::vec4(agentColor.r(), agentColor.g(), agentColor.b(), agentColor.a())});
                //lines.push_back({currentPoint, glm::vec4(linesColor.r(), linesColor.g(), linesColor.b(), 1.0f)});

                lastPoint = currentPoint;
                count++;
            }
            */
/*
            glm::vec3 normal =agent.getCurrentFace()->getNormal();
            // Also draw the triangle of the current face of the agent
            //auto vertices = agent.getCurrentFace()->getVertices();
            glm::vec3 prevVertex3D = convertTo3D(projectTo2D(vertices.back()->position, agent.getCurrentFace()), planeZ);

            for (auto& vertex : vertices) {
                glm::vec3 vertex3D = convertTo3D(projectTo2D(vertex->position, agent.getCurrentFace()), planeZ);
                points.push_back({prevVertex3D, glm::vec4(faceColor.r(), faceColor.g(), faceColor.b(), faceColor.a())});
                points.push_back({vertex3D, glm::vec4(faceColor.r(), faceColor.g(), faceColor.b(), faceColor.a())});
                prevVertex3D = vertex3D;
            }
        */
        
            auto pointsCurrentTriangle=drawSmallerTriangle( agent.getCurrentFace(), 0.17f, planeZ, faceColor.vec3());
            auto pointsNextTriangle=   drawSmallerTriangle( agent.getNextFace(), 0.17f, planeZ, nextFaceColor.vec3());
            // Now add the returned points to the main 'points' vector
            points.insert(points.end(), pointsCurrentTriangle.begin(), pointsCurrentTriangle.end());
            points.insert(points.end(), pointsNextTriangle.begin(), pointsNextTriangle.end());
        }

        return points;
    }

    // Function that calculates and returns the points for a smaller triangle
    std::vector<DrawSupport::PointInfo> SurfRender::drawSmallerTriangle(Face* face, float scale, float planeZ, glm::vec3 faceColor) {
        std::vector<DrawSupport::PointInfo> points;
        auto vertices = face->getVertices();

        // Calculate the centroid of the triangle
        glm::vec3 centroid(0.0f);
        for (auto& vertex : vertices) {
            centroid += vertex->position;
        }
        centroid /= vertices.size();

        glm::vec3 prevVertex3D;
        glm::vec3 firstVertex3D;
        bool isFirstVertex = true;

        for (auto& vertex : vertices) {
            glm::vec3 originalPos3D = vertex->position;
            glm::vec3 vertex3D = convertTo3D(projectTo2D(originalPos3D, face), planeZ);

            // Calculate the new position of each vertex moving towards the centroid
            glm::vec3 movedVertex3D  = originalPos3D - scale * (originalPos3D-centroid);
           // glm::vec3 movedVertex3D = convertTo3D(projectTo2D(movedVertex, face), planeZ);

            // Create a point for the smaller triangle
            DrawSupport::PointInfo point;
            point.position = movedVertex3D;
            point.color = faceColor;

            // Store the vertices for drawing
            if (!isFirstVertex) {
                 points.push_back({prevVertex3D, faceColor}); // Add line segment from previous vertex to this one
                 points.push_back({movedVertex3D, faceColor}); 
            } else {
                firstVertex3D = movedVertex3D;  // Save the first vertex to close the loop later
                isFirstVertex = false;
            }
            //points.push_back(point);
            prevVertex3D = movedVertex3D;
        }

        // Close the loop by connecting the last vertex to the first
        points.push_back({prevVertex3D, faceColor});
        points.push_back({firstVertex3D, faceColor});

        return points;
    }

    glm::vec3 SurfRender::convertTo3D(const glm::vec2& point2D, float planeZ) {
        return glm::vec3(point2D.x, point2D.y, planeZ);
    }
    


}