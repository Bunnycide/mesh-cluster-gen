#ifndef DISTORTIONCHECKER_H_
#define DISTORTIONCHECKER_H_

#include "pmp/visualization/mesh_viewer.h"

inline auto VERTEX_DISTORTION_ATTRIBUTE = "v:distortion";

struct VertexHash {
    size_t operator()(const pmp::Vertex& v) const {
        return std::hash<int>()(v.idx());  // Using the vertex index for hashing
    }
};

/**
 *@brief
 * Measures how much the area around a vertex nn in the mesh deviates from being flat (developable),
 * both at a specific scale rr and across multiple scales. Which is valuable in identifying areas with high distortion.
* */
void calculate_region_distortion(pmp::SurfaceMesh &mesh, int region_radius);

/**
 * @brief Calculate regional distortion for the given vertex.
 */
double calculate_vertex_distortion(const pmp::SurfaceMesh &mesh, const pmp::Vertex &vertex, const int &region_radius);


// Function to find boundary vertices within a given radius r
std::vector<pmp::Vertex> find_boundary_vertices(const pmp::SurfaceMesh& mesh, pmp::Vertex start_vertex, int r);

/**
 * @param mesh The loaded mesh to process
 * @param v1 First of the two vertices to calculate distance between
 * @param v2 Second of the two vertices to calculate distance between
 * @return The distance between the two vertices
 */
double calculate_distance(const pmp::SurfaceMesh& mesh, pmp::Vertex v1, pmp::Vertex v2);

/**
 * @brief Calculate angle between vectors ⃗v1 = p1p0 and ⃗v2 = p1p2
 * @param p0 p0 in ⃗v1
 * @param p1 p1 in ⃗v2 and ⃗v1
 * @param p2 p2 in ⃗v2
 */
double angle_between(pmp::Point p0, pmp::Point p1, pmp::Point p2);

/**
 * @brief convert regional vertex distortion values to texture coordinates
 * @param mesh Surface Mesh with vertex distortion values to generate the texture coordinates for
 */
void region_distortion_to_texture_coordinates(pmp::SurfaceMesh &mesh);


#endif