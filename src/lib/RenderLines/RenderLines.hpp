// Operations.hpp
#ifndef RENDERLINES_HPP
#define RENDERLINES_HPP

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

    std::vector<Face*> updateFrontFaces(HalfedgeDS& structD, Scene::Camera& camera);

    std::unordered_set<Face*> updateFrontFacesSet(HalfedgeDS &structD, Scene::Camera &camera);

    std::vector<DrawSupport::PointInfo> renderSilhouetteLines(HalfedgeDS& halfedgeDS, Scene::Camera& camera);

}

#endif // RENDERLINES_HPP
