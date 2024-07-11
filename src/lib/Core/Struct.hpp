#pragma once
#ifndef STRUCT_HPP
#define STRUCT_HPP

// Forward declarations to avoid any potential issues in individual headers
class Vertex;
class Halfedge;
class Face;

// Only include Geometry or other non-circular dependencies here


#include "Vertex.hpp"
#include "Halfedge.hpp"
#include "Face.hpp"

#include "Geometry.hpp"

#endif // STRUCT_HPP
