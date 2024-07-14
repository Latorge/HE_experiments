#pragma once

#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <glm/glm.hpp>

// Forward declarations to avoid any potential issues in individual headers
class Vertex;
class Halfedge;
class Face;
class FreeHalfedgeIterator;
//class HalfedgeDS

// Only include Geometry or other non-circular dependencies here


#include "Vertex.hpp"
#include "Halfedge.hpp"
#include "Face.hpp"
#include "Geometry.hpp"
//#include "HalfedgeDS.hpp"

#endif // STRUCT_HPP
