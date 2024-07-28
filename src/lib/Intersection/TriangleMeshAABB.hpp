// createOperations.hpp
#ifndef TRIANGLEMESHAABB_HPP
#define TRIANGLEMESHAABB_HPP

#include "../Operations/Operations.hpp"

#include <iostream>
#include <list>
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include "glm/vec3.hpp"  // Ensure you have glm included in your project

namespace intersections {

    typedef CGAL::Simple_cartesian<float> Kernel;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Triangle_3 Triangle;
    typedef std::list<Triangle>::iterator Iterator;
    typedef CGAL::AABB_triangle_primitive<Kernel, Iterator> Primitive;
    typedef CGAL::AABB_traits<Kernel, Primitive> AABB_triangle_traits;
    typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;

    typedef Kernel::Point_3 Point;
    typedef Kernel::Plane_3 Plane;
    typedef Kernel::Vector_3 Vector;
    typedef Kernel::Segment_3 Segment;
    typedef Kernel::Ray_3 Ray;

    typedef boost::optional< Tree::Intersection_and_primitive_id<Segment>::Type > Segment_intersection;
    typedef boost::optional< Tree::Intersection_and_primitive_id<Plane>::Type > Plane_intersection;
    typedef Tree::Primitive_id Primitive_id;

    class TriangleMeshAABB {
    public:
        TriangleMeshAABB();
        void addTrianglesFromBuffer(const std::vector<glm::vec3>& buffer);
        int countIntersections(const Kernel::Ray_3& ray);
        Point closestPoint(const Point& query);
        double squaredDistance(const Point& query);

    private:
        std::list<Triangle> triangles;
        Tree tree;
    };







}

#endif // TRIANGLEMESHAABB_HPP