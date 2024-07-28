#include "TriangleMeshAABB.hpp"

namespace intersections {

    TriangleMeshAABB::TriangleMeshAABB() {
        // Empty constructor for now. Tree initialization can be lazy.
    }

    void TriangleMeshAABB::addTrianglesFromBuffer(const std::vector<glm::vec3>& buffer) {
        for (size_t i = 0; i + 2 < buffer.size(); i += 3) {
            Point a(buffer[i].x, buffer[i].y, buffer[i].z);
            Point b(buffer[i+1].x, buffer[i+1].y, buffer[i+1].z);
            Point c(buffer[i+2].x, buffer[i+2].y, buffer[i+2].z);
            triangles.push_back(Triangle(a, b, c));
        }
        tree.rebuild(triangles.begin(), triangles.end()); // Rebuild the tree whenever new triangles are added
    }

    int TriangleMeshAABB::countIntersections(const Kernel::Ray_3& ray) {

        
        return tree.number_of_intersected_primitives(ray);
    }

    Point TriangleMeshAABB::closestPoint(const Point& query) {
        return tree.closest_point(query);
    }

    double TriangleMeshAABB::squaredDistance(const Point& query) {
        return tree.squared_distance(query);
    }

}


/*
  TriangleMesh mesh;
    // Suppose this is your buffer of glm::vec3 points:
    std::vector<glm::vec3> points = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
                                     {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0}};
    mesh.addTrianglesFromBuffer(points);

    // Example query
    Kernel::Ray_3 ray_query(Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
    std::cout << mesh.countIntersections(ray_query) << " intersection(s) with ray query" << std::endl;

    Point point_query(2.0, 2.0, 2.0);
    std::cout << "Closest point is: " << mesh.closestPoint(point_query) << std::endl;
    std::cout << "Squared distance: " << mesh.squaredDistance(point_query) << std::endl;
*/