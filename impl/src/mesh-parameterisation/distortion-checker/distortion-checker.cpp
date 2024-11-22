#include "mesh-parameterisation/distortion-checker/distortion-checker.h"

double angle_between(const pmp::Point v0, const pmp::Point v1, const pmp::Point v2)
{
    // Calculate vectors from v1 to v0 and from v1 to v2
    pmp::Point vector1 = v0 - v1;
    pmp::Point vector2 = v2 - v1;

    // Calculate the dot product of the vectors
    double dot_product = dot(vector1, vector2);

    // Calculate the magnitudes (lengths) of the vectors
    double magnitude1 = norm(vector1);
    double magnitude2 = norm(vector2);

    // Calculate the cosine of the angle
    double cos_theta = dot_product / (magnitude1 * magnitude2);

    // Clamp the result to the range [-1, 1] to avoid numerical issues with acos
    cos_theta = std::clamp(cos_theta, -1.0, 1.0);

    // Calculate and return the angle in radians
    return std::acos(cos_theta);
}

double calculate_distance(  const pmp::SurfaceMesh& mesh,
                            const pmp::Vertex v1,
                            const pmp::Vertex v2) {
    // Get the positions of the two vertices
    pmp::Point p1 = mesh.position(v1);
    pmp::Point p2 = mesh.position(v2);

    // Calculate the Euclidean distance between the two points
    const double distance = std::sqrt(
        std::pow(p2[0] - p1[0], 2) +
        std::pow(p2[1] - p1[1], 2) +
        std::pow(p2[2] - p1[2], 2)
    );

    return distance;
}


std::vector<pmp::Vertex> find_boundary_vertices(const pmp::SurfaceMesh& mesh,
                                                const pmp::Vertex start_vertex,
                                                const int r)
{
    std::vector<pmp::Vertex> visited; // Store visited vertices
    std::vector<pmp::Vertex> current_layer = { start_vertex }; // The starting vertex layer
    std::vector<pmp::Vertex> boundary_vertices; // To hold the final boundary vertices

    visited.push_back(start_vertex); // Mark the start vertex as visited

    for (int i = 0; i < r; ++i)
    {
        std::vector<pmp::Vertex> next_layer; // To store the next layer of vertices

        // Traverse the current layer of vertices
        for (auto vertex : current_layer)
        {
            // Iterate over each halfedge of the current vertex
            for (auto edge : mesh.halfedges(vertex))
            {
                pmp::Vertex neighbor = mesh.to_vertex(edge);

                // Check if the neighbor has already been visited
                if (std::find(visited.begin(), visited.end(), neighbor) == visited.end())
                {
                    visited.push_back(neighbor); // Mark this vertex as visited
                    next_layer.push_back(neighbor); // Add it to the next layer
                }
            }
        }

        // Move to the next layer
        current_layer = std::move(next_layer);
    }

    // After r layers, `current_layer` holds the boundary vertices at distance `r`
    boundary_vertices.insert(boundary_vertices.end(),
                                current_layer.begin(),
                                current_layer.end());

    return boundary_vertices;
}

double calculate_vertex_distortion(const pmp::SurfaceMesh &mesh, const pmp::Vertex &vertex, const int &region_radius)
{
    // Find the longest edge from the vertex in region radius
    auto boundary_vertices = find_boundary_vertices(mesh, vertex, region_radius);
    auto positions = mesh.get_vertex_property<pmp::Point>("v:point");
    double angle_aggregate  = 0.0;
    int  j;
    for(int i = 0; i < boundary_vertices.size(); i++)
    {
        j = i+1;
        if(j >= boundary_vertices.size()) j = 0;
        angle_aggregate += angle_between(  positions[vertex],
                                positions[boundary_vertices[i]],
                                positions[boundary_vertices[j]]);
    }

    // Angle distortion
    return  M_2_PI - angle_aggregate / M_2_PI;
}

void calculate_region_distortion(pmp::SurfaceMesh &mesh, int region_radius)
{
    auto vertex_distortion =
        mesh.add_vertex_property<pmp::Scalar>(VERTEX_DISTORTION_ATTRIBUTE,
                                                std::numeric_limits<float>::infinity());

    // Iterate over the vertices
    auto r_region_magnitude = region_radius;
    float regional_distortion = 0;

    // Iterate over vertices
    for(auto v : mesh.vertices())
    {
        regional_distortion = static_cast<float>(calculate_vertex_distortion(   mesh,
                                                                                v,
                                                                                r_region_magnitude));

        vertex_distortion[v] = regional_distortion;
    }
}

void region_distortion_to_texture_coordinates(pmp::SurfaceMesh &mesh)
{
    auto vertex_distortion = mesh.get_vertex_property<pmp::Scalar>(VERTEX_DISTORTION_ATTRIBUTE);

    for(auto v : mesh.vertices())
    {
        vertex_distortion[v] = -(vertex_distortion[v] * 180.0 / M_PI ) / 360.0 ;
    }

    // sort distortion values
    std::vector<pmp::Scalar> values;
    values.reserve(mesh.n_vertices());

    for(const auto v : mesh.vertices())
    {
        values.push_back( vertex_distortion[v]);
    }

    std::ranges::sort(values);
    unsigned int n = values.size() - 1;

    unsigned int i = n / 100;
    const pmp::Scalar kmin = values[i], kmax = values[n - 1];

    auto tex = mesh.vertex_property<pmp::TexCoord>("v:tex");

    for(auto v : mesh.vertices())
    {
        tex[v] = pmp::TexCoord(
            (vertex_distortion[v] - kmin) / (kmax - kmin),
            0.0
            );
    }
}