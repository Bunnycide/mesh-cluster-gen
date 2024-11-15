#include <queue>

#include "cwd/cwd.h"
#include "pmp/io/read_obj.h"
#include "distortion-checker/distortion-checker.h"

int main() {
    pmp::SurfaceMesh mesh;

    // Load the mesh from file
    pmp::read_obj(mesh, CWD "/assets/models/common-3d-test-models/data/beast.obj");
    calculate_region_distortion(mesh, 1);

    auto distortion_values = mesh.get_vertex_property<pmp::Point>("v:distortion");

    auto mesh_viewer = new pmp::MeshViewer("default", 800, 600);


    mesh_viewer->run();
    return 0;
}
