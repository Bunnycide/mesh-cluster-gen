#ifndef D_CHARTS_H_
#define D_CHARTS_H_

#include "pmp/surface_mesh.h"
#include <vector>

typedef struct Priority_
{
    unsigned int    cost,
                    triangle_handle,
                    chart_index;
} Priority;

class DCharts
{
public:
    explicit DCharts() = default;
    explicit DCharts(pmp::SurfaceMesh* mesh);

    void set_surface_mesh(pmp::SurfaceMesh* mesh);
    void segment() const;

private:
    pmp::SurfaceMesh* m_mesh                        {};

    // The charts as a list of faces
    mutable std::vector<std::vector<pmp::Face>>     charts;
    // Seeds to grow a chart from
    mutable std::vector<pmp::Face>                  chart_seeds;
    // Priority queue for each chart
    mutable std::vector<Priority>                   priorities;
    // Axis of the union of cones
    mutable std::vector<pmp::Normal>                Nc_list;
    // Deviation angle from conic axis
    mutable std::vector<pmp::Scalar>                theta_c_list;
    // Chart area list
    mutable std::vector<pmp::Scalar>                chart_areas_list;

    // Cost function variables
    float   developability                          {0.0f},                     // Defines how flat the final chart is
            compactness_round_boundaries            {0.0f},                     // Defines final chart conformance to a round shape
            compactness_straight_boundaries         {0.0f};                     // Defines final chart conformance to a straight edge boundary

    // Cost function importance fractions
    float   alpha_developability                    {1.0f},
            beta_compactness_round_boundaries       {0.7f},
            gamma_compactness_straight_boundaries   {0.5f};

    // Shorthands for cost function fractions
    float   *alpha  = &alpha_developability,
            *beta = &beta_compactness_round_boundaries,
            *gamma = &gamma_compactness_straight_boundaries;

    // The cost of adding a triangle to a chart
    float cost{0.0f};

    void    calculate_cost_function();
    float   calculate_fitting_error(unsigned int index, pmp::Normal nt) const;
    float   calculate_disc_compactness(int chart_index, pmp::Face face);
    float   calculate_straightness(std::vector<pmp::Face> &chart, pmp::Face &candidate);

    // Modified lloyd iterations
    void    select_initial_seed_triangles(int num_seeds) const;

    float   calculate_fitting_error() const;

    void    grow_charts() const;

    [[deprecated("This function is not yet implemented")]] void find_new_proxies();
    [[deprecated("This function is not yet implemented")]] void find_new_seeds();

    // Hole filling
    [[deprecated("This function is not yet implemented")]] void identify_holes();
    [[deprecated("This function is not yet implemented")]] void close_holes();

    // Post-processing
    [[deprecated("This function is not yet implemented")]] void run_post_processing();
};

#endif // D_CHARTS_H_

