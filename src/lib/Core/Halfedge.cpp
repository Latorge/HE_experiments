#include "Halfedge.hpp"

Halfedge::Halfedge(Vertex* vertex, int _id) : vertex(vertex), id(_id) {
    // Initialize all pointers to nullptr to avoid undefined behavior
        face = nullptr;
        twin = nullptr;
        prev = nullptr;
        next = nullptr;
}


std::string Halfedge::getId() const {
    return std::to_string(vertex->id) + '-' + std::to_string(twin->vertex->id);
}

std::string Halfedge::getIdReverse() const {
    return std::to_string(twin->vertex->id) + '-' + std::to_string(vertex->id);
}

bool Halfedge::containsPoint(const glm::vec3& point, float tolerance = 1e-10) {
    glm::vec3 closest = closestPointOnLine(vertex->position, next->vertex->position, point);
    return glm::length(closest - point) < tolerance;
}

bool Halfedge::isFree() const {
    return face == nullptr;
}

bool Halfedge::isBoundary() const {
    return face == nullptr && twin && twin->face != nullptr;
}

bool Halfedge::isConcave() const {
    if (twin) {
        return Geometry::frontSide(vertex->position, next->vertex->position, prev->vertex->position, twin->prev->vertex->position) > 0;
    }
    return false;
}

void Halfedge::forEachHalfedgeCW(const std::function<void(Halfedge*)>& action) {
    Halfedge* start = this;
    Halfedge* current = start;
    do {
        action(current);
        current = current->next;
    } while (current != start);
}

void Halfedge::forEachHalfedgeCCW(const std::function<void(Halfedge*)>& action) {
    Halfedge* start = this;
    Halfedge* current = start;
    do {
        action(current);
        current = current->prev;
    } while (current != start);
}

 // Generate a vector of all halfedges in this loop
std::vector<Halfedge*> Halfedge::nextLoop() {
    std::vector<Halfedge*> loop;
    Halfedge* start = this;
    do {
        loop.push_back(start);
        start->visited = true;
        start = start->next;
    } while (start != this && start != nullptr);
    return loop;
}


glm::vec3 Halfedge::closestPointOnLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p) {
    glm::vec3 ap = p - a;
    glm::vec3 ab = b - a;
    float ab2 = glm::dot(ab, ab);
    float ap_ab = glm::dot(ap, ab);
    float t = ap_ab / ab2;
    return a + ab * glm::max(0.f, glm::min(1.f, t));  // Clamping t to the segment
}



FreeHalfedgeIterator::FreeHalfedgeIterator(std::vector<Halfedge*>::iterator start, std::vector<Halfedge*>::iterator end)
    : current(start), end(end) {
    while (current != end && !(*current)->isFree()) {
        ++current;
    }
}

bool FreeHalfedgeIterator::hasNext() const {
    return current != end;
}

Halfedge* FreeHalfedgeIterator::next() {
    if (current == end) return nullptr;
    Halfedge* result = *current;
    do {
        ++current;
    } while (current != end && !(*current)->isFree());
    return result;
}

