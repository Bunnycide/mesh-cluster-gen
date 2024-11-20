
// Copyright 2011-2019 the Polygon Mesh Processing Library developers.
// Distributed under a MIT-style license, see LICENSE.txt for details.

#include <cwd/cwd.h>
#include <pmp/visualization/mesh_viewer.h>
#include <pmp/algorithms/curvature.h>
#include <imgui.h>
#include "mesh-parameterisation/distortion-checker/distortion-checker.h"

using namespace pmp;

class Viewer : public MeshViewer
{
public:
    Viewer(const char* title, int width, int height, bool showgui);

protected:
    void process_imgui() override;
};

Viewer::Viewer(const char* title, int width, int height, bool showgui)
    : MeshViewer(title, width, height, showgui)
{
    set_draw_mode("Solid Smooth");
}

void Viewer::process_imgui()
{
    MeshViewer::process_imgui();

    if (ImGui::CollapsingHeader("Curvature", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static int region_radius = 1;
        ImGui::SliderInt("Region radius", &region_radius, 0, 10);

        if(ImGui::Button("Regional Vertex Distortion"))
        {
            calculate_region_distortion(mesh_, region_radius);
            region_distortion_to_texture_coordinates(mesh_);
            renderer_.use_cold_warm_texture();
            update_mesh();
            set_draw_mode("Texture");
        }

        if(ImGui::Button("Generate Seam"))
        {

        }
    }
}

int main(int argc, char** argv)
{
#ifndef __EMSCRIPTEN__
    Viewer window("Regional distortion", 800, 600, true);
    window.load_mesh(CWD "/assets/models/common-3d-test-models/data/cow.obj");
    return window.run();
#else
    Viewer window("Curvature", 800, 600, true);
    window.load_mesh(argc == 2 ? argv[1] : "input.off");
    return window.run();
#endif
}
