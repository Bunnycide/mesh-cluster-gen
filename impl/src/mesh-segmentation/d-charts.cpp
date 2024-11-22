#include "mesh-segmentation/d-charts.h"
#include <random>
#include "mesh-segmentation/utils.h"
#include "pmp/algorithms/normals.h"

DCharts::DCharts(pmp::SurfaceMesh* mesh) : m_mesh(mesh)
{
    // Do algorithm initialisation here
}

void DCharts::set_surface_mesh(pmp::SurfaceMesh* mesh)
{
    // Set per face color to cornflour blue
    m_mesh = mesh;
}

void DCharts::segment() const
{
    select_initial_seed_triangles(4);
    grow_charts();
}

void DCharts::calculate_cost_function()
{
    cost =  std::pow(developability, *alpha)
        *   std::pow(compactness_round_boundaries, *beta)
        *   std::pow(compactness_straight_boundaries, *gamma);
}

float DCharts::calculate_fitting_error(const unsigned int index, const pmp::Normal nt) const
{
    return std::pow((pmp::dot(Nc_list[index], nt) - std::cos(theta_c_list[index]) ) , 2.0f);
}

float DCharts::calculate_disc_compactness(int chart_index, pmp::Face face)
{
    return 0.0f;
}

float DCharts::calculate_straightness(std::vector<pmp::Face>& chart, pmp::Face& candidate)
{
    return 0.0f;
}

void DCharts::select_initial_seed_triangles(const int num_seeds) const
{
    auto i = 0;

    // Get chart seed index handles
    auto chart_seed_indices = pick_seed_faces(m_mesh, num_seeds);

    // Get faces from chart seed handles
    chart_seeds.resize(chart_seed_indices.size());

    for (auto& chart_seed : chart_seeds)
    {{
            chart_seed = pmp::Face(chart_seed_indices[i]);
    }i++;}

    charts.resize(chart_seeds.size());

    i = 0;
    // Initialise charts with seed triangles
    for (auto &chart : charts){
    {
        chart.push_back(chart_seeds.at(i));
    }i++;}
}

void DCharts::grow_charts() const
{
    // grow_chart(m_mesh, charts[0]);
    unsigned int i = 0; // Index to check inside the priority queue

    // calculate initial axis of union of cone
    for (const auto &seed : chart_seeds)
    {
        Nc_list.push_back(face_normal(*m_mesh, seed));
    }

    // Add the first adjacent triangles to the priority queue
    
}

