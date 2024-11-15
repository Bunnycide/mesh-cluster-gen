#ifndef DISTORTIONCHECKER_H_
#define DISTORTIONCHECKER_H_

#include "pmp/visualization/mesh_viewer.h"

struct VertexHash {
    size_t operator()(const pmp::Vertex& v) const {
        return std::hash<int>()(v.idx());  // Using the vertex index for hashing
    }
};

void calculate_region_distortion(pmp::SurfaceMesh &mesh, int region_radius);
double calculate_vertex_distortion(const pmp::SurfaceMesh &mesh, const pmp::Vertex &vertex, const int &region_radius);
std::vector<pmp::Vertex> find_boundary_vertices(const pmp::SurfaceMesh& mesh, pmp::Vertex start_vertex, int r);
double calculate_distance(const pmp::SurfaceMesh& mesh, pmp::Vertex v1, pmp::Vertex v2);
double angle_between(pmp::Point v0, pmp::Point v1, pmp::Point v2);

#endif