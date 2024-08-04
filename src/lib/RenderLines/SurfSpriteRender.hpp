// Operations.hpp
#ifndef SURFSPRITERENDER_HPP
#define SURFSPRITERENDER_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <random>

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/range.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp> // For length2 function
#include <glm/gtx/rotate_vector.hpp> // For rotate functions
#include <cmath>

#include "../DrawSupport/drawSupport.hpp"
#include "../Scene/Scene.hpp"

#include "surfSupportFunctions.hpp"
#include "RenderLines.hpp"
#include "triangeFrame.hpp"
#include "surfAgent.hpp"

#include "../Core/Vertex.hpp"
#include "../Core/Halfedge.hpp"
#include "../Core/Face.hpp"
#include "../Core/HalfedgeDS.hpp"

#include <unordered_set>


namespace RenderLines {

    class SurfRender {
    public:
        SurfRender(HalfedgeDS* halfedgeDS, int initNumberAgents);

        void initializeAgents(int count);

        void update(float deltaTime);

        void render();

        std::vector<DrawSupport::PointInfo> collectTrailSegments();

        bool isAtBoundary(const SurfAgent &agent);

        std::vector<DrawSupport::PointInfo> drawProjectedAgentsAndFaces(float planeZ);

        glm::vec3 convertTo3D(const glm::vec2 &point2D, float planeZ);
        std::vector<DrawSupport::PointInfo> drawSmallerTriangle(Face *face, float scale, float planeZ, glm::vec3 faceColor);

    private:
        HalfedgeDS* halfedgeDS;
        std::vector<SurfAgent> agents;

        // Example function to check if an agent is at a boundary
    };

}

#endif // SURFSPRITERENDER_HPP