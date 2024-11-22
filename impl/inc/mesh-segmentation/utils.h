#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <pmp/surface_mesh.h>

#define NUM_VERTICES_TRIANGLE 3

pmp::Point compute_centroid(pmp::SurfaceMesh* mesh, pmp::Face face);
std::vector<unsigned int> pick_seed_faces(pmp::SurfaceMesh* mesh, int k);

std::vector<unsigned int> grow_chart(pmp::SurfaceMesh* mesh, std::vector<unsigned int> &chart);

#endif //UTILS_H_
