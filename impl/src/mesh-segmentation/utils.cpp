#include "mesh-segmentation/utils.h"

[[nodiscard]] pmp::Point compute_centroid(pmp::SurfaceMesh* mesh, pmp::Face face)
{
    pmp::Point centroid;

    for (auto vertex : mesh->vertices(face))
    {
        centroid += mesh->position(vertex);
    }
    centroid /= NUM_VERTICES_TRIANGLE;

    return centroid;
}

// Seed faces are picked as the farthest points in a point cloud of face centroids using the farthest point sampling
std::vector<unsigned int> pick_seed_faces(pmp::SurfaceMesh* mesh, int k)
{
    unsigned int n = mesh->n_faces();
    if (k > n)
    {
        throw std::invalid_argument("k must be less than the number of vertices");
    }

    // Step 1: Compute centroids for all the faces
    std::vector<pmp::Point> centroids;
    std::vector<pmp::IndexType> face_idxs;
    for (const auto f : mesh->faces())
    {
        centroids.push_back(compute_centroid(mesh, f));
        face_idxs.push_back(f.idx());
    }

    std::vector<unsigned int> selected_indices; // Indices of selected faces
    std::vector distances(n, std::numeric_limits<double>::max());

    selected_indices.push_back(0);

    for (int i = 0; i < k; ++i){
        // Update distances for all centroids
        unsigned int last_selected = selected_indices.back();
        for (int j = 0; j < n; ++j)
        {
            double dist = pmp::distance(centroids[last_selected], centroids[j]);
            distances[j] = std::min(distances[j], dist);
        }

        // Find the centroid with the maximum distance
        int next_point = -1;
        double max_dist = -1.0;
        for (int j = 0; j < n; ++j)
        {
            if (distances[j] > max_dist)
            {
                max_dist = distances[j];
                next_point = j;
            }
        }

        selected_indices.push_back(face_idxs[next_point]);
    }

    // Remove the 0 index
    selected_indices.erase(selected_indices.begin());

    return selected_indices;
}

std::vector<unsigned int> grow_chart(pmp::SurfaceMesh* mesh, std::vector<unsigned int>& chart)
{

    // TODO: IMP: Please replace this with actual return statement that returns a grown chart
    return {};
}
