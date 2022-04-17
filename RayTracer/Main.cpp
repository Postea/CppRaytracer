
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "Config.h"
#include "material/BackgroundMaterial.h"
#include "material/DiffuseMaterial.h"
#include "material/texture/Checkerboard.h"
#include "material/texture/Constant.h"
#include "sampling/Image.h"
#include "sampling/Scene.h"
#include "shape/CirclePlane.h"
#include "shape/Group.h"
#include "shape/LightSingleGroup.h"
#include "shape/RectanglePlane.h"
#include "shape/ShapeSingleGroup.h"
#include "shape/SkySphere.h"
#include "shape/Sphere.h"
#include "shape/TriangleMesh.h"
#include "testing/Testing.h"
#include "tools/Mat4.h"
#include "tools/ObjectLoader.h"
#include "tools/Random.h"

using namespace material;
using namespace util;
using namespace test;
using namespace cam;
using namespace shapes;
using namespace std;

int tree_recurs(const std::vector<shapes::TriMeshNode>& hierarchy,
                shapes::TriMeshNode r) {
	int ld = 1;
	int rd = 1;
	if (r.left != -1) {
		ld += tree_recurs(hierarchy, hierarchy[r.left]);
	}
	if (r.right != -1) {
		rd += tree_recurs(hierarchy, hierarchy[r.right]);
	}
	int d = max<int>(rd, ld);
	// if (d != 0) cout << d << endl;
	return d;
}
int main() {
#if false
	cout << "Start" << endl;
	// Image img = Image (100, 100);

	Mat4 ident;
	CamObs obs(config::camera, M_PI / 2, 600, 360);

	Group group(ident);

	Image sky_image = readImage(config::sky.c_str());
	sky_image.halfImage(true, 0.01);
	auto skysphere = make_shared<SkySphere>(
	    SkySphere(make_shared<Image>(sky_image), sky_image, 0.2f));
	// auto skysphere =
	// make_shared<SkySphere>(make_shared<Constant>(Vec3(0.9)));

	group.add(skysphere);

	auto floor = make_shared<RectanglePlane>(
	    1000.0f, 1000.0f, false, make_shared<DiffuseMaterial>(Vec3(0.3)));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1.001, 0)), floor));

	auto checkered_board = make_shared<RectanglePlane>(
	    16.0f, 16.0f, false,
	    make_shared<DiffuseMaterial>(
	        make_shared<Checkerboard>(8, Vec3(1), Vec3(0.9))));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1, 0)), checkered_board));

	TriangleMesh rook(std::ifstream("Tower_Base.obj"),
	                  make_shared<DiffuseMaterial>(Vec3(0.05f, 0.05f, 0.9f)));
	TriangleMesh cube(std::ifstream("Cube.obj"),
	                  make_shared<DiffuseMaterial>(Vec3(0.05f, 0.9f, 0.05f)));

	group.add(ShapeSingleGroup(translate(Vec3(0, 0, 0)),
	                           make_shared<TriangleMesh>(rook)));
	group.add(ShapeSingleGroup(translate(Vec3(3, 0, 0)),
	                           make_shared<TriangleMesh>(cube)));

	auto red_sphere = make_shared<Sphere>(
	    1.0f, make_shared<DiffuseMaterial>(Vec3(0.9f, 0.05f, 0.05f)));
	group.add(ShapeSingleGroup(translate(Vec3(-3, 1, 0)), red_sphere));

	std::vector<std::shared_ptr<Light>> lights = {skysphere};
	auto sc = std::make_shared<Scene>(
	    Scene(group, lights, obs, config::max_depth, config::sample_l));

	clock_t clkStart;
	clock_t clkFinish;
	cout << "Start render" << endl;
	clkStart = clock();
	Image img = raytrace(config::threadpool_size, sc, config::sample_n);
	clkFinish = clock();
	cout << "Start imaging to " << config::fnme << endl;
	writeBmp(config::fnme.c_str(), img);
	cout << "End" << endl;
	std::cout << clkFinish - clkStart;

#elif false
	// test::vec3_test();
	// test::mat4_test();
	// test::ray_test();
	// test::shape_test();
	// test::axisalignedboundingbox_test();

#elif false
	Image x = readImage("results/mis_2.bmp");
	x.halfImage(true, 0.2);
	cout << "halfed" << endl;
	writeBmp("results/aaa.bmp", x);
#elif true
	std::ifstream is("Cube.obj");
	TriangleMesh mesh(is, nullptr);
	cout << "leaves: " << mesh.leaves.size() << endl;
	cout << "hierarchy: " << mesh.hierarchy.size() << endl;
	int i = 0;
	for (auto hier : mesh.hierarchy) {
		cout << i << " {" << hier.left << " " << hier.right << " "
		     << hier.leaves_i << " " << hier.leaves_size << "}" << endl;
		i++;
		for (int i = 0; i < hier.leaves_size; i++) {
			cout << mesh.leaves[hier.leaves_i + i].p1.position << " "
			     << mesh.leaves[hier.leaves_i + i].p2.position << " "
			     << mesh.leaves[hier.leaves_i + i].p3.position << endl;
		}
	}
	cout << endl;
	for (auto hier : mesh.hierarchy) {
		std::vector<util::Vertex> v;
		std::vector<std::tuple<int, int, int> > f;
		for (int i = 0; i < hier.leaves_size; i++) {
			auto tri = mesh.leaves[hier.leaves_i + i];
			int p1_i = -1;
			int p2_i = -1;
			int p3_i = -1;
			for (int j = 0; j < v.size(); j++) {
				auto p = v[j];
				if (p.position == tri.p1.position) p1_i = j + 1;
				if (p.position == tri.p2.position) p2_i = j + 1;
				if (p.position == tri.p3.position) p3_i = j + 1;
			}
			if (p1_i == -1) {
				v.push_back(tri.p1);
				p1_i = v.size();
			}
			if (p2_i == -1) {
				v.push_back(tri.p2);
				p2_i = v.size();
			}
			if (p3_i == -1) {
				v.push_back(tri.p3);
				p3_i = v.size();
			}
			f.push_back({p1_i, p2_i, p3_i});
		}
		for (auto x : v) {
			cout << "v " << x.position[0] << ".000000 " << x.position[1]
			     << ".000000 " << x.position[2] << ".000000" << endl;
		}
		for (auto x : f) {
			cout << "f " << std::get<0>(x) << " " << std::get<1>(x) << " "
			     << std::get<2>(x) << endl;
		}
		cout << endl;
	}

	cout << tree_recurs(mesh.hierarchy, mesh.hierarchy[0]) << endl;
#endif
};