
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

int main() {
#if true
	cout << "Start" << endl;
	// Image img = Image (100, 100);

	Mat4 ident;
	CamObs obs(config::camera, M_PI / 2, 600, 360);

	Group group(ident);

	Image sky_image = readImage(config::sky.c_str());
	sky_image.halfImage(true, 0.01);
	auto skysphere = make_shared<SkySphere>(
	    SkySphere(make_shared<Image>(sky_image), sky_image));
	// auto skysphere =
	// make_shared<SkySphere>(make_shared<Constant>(Vec3(0.9)));

	group.add(skysphere);

	auto floor = make_shared<RectanglePlane>(
	    1000.0f, 1000.0f, false, make_shared<DiffuseMaterial>(Vec3(0.3)));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1.001, 0)), floor));

	auto checkered_board = make_shared<RectanglePlane>(
	    16.0f, 16.0f, false,
	    make_shared<DiffuseMaterial>(make_shared<Checkerboard>(8)));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1, 0)), checkered_board));

	std::ifstream roo("Tower_Base.obj");
	TriangleMesh rook(roo,
	                  make_shared<DiffuseMaterial>(Vec3(0.05f, 0.05f, 0.9f)));
	group.add(ShapeSingleGroup(translate(Vec3(-1, 0, 1)),
	                           make_shared<TriangleMesh>(rook)));

	auto red_sphere = make_shared<Sphere>(
	    1.0f, make_shared<DiffuseMaterial>(Vec3(0.9f, 0.05f, 0.05f)));
	group.add(ShapeSingleGroup(translate(Vec3(7, 0, -7)), red_sphere));
	group.add(ShapeSingleGroup(translate(Vec3(7, 0, 7)), red_sphere));

	auto green_sphere = make_shared<Sphere>(
	    1.0f, make_shared<DiffuseMaterial>(Vec3(0.05f, 0.9f, 0.05f)));
	group.add(ShapeSingleGroup(translate(Vec3(-7, 0, -7)), green_sphere));
	group.add(ShapeSingleGroup(translate(Vec3(-7, 0, 7)), green_sphere));

	std::vector<std::shared_ptr<Light>> lights = {skysphere};
	auto sc = std::make_shared<Scene>(
	    Scene(group, lights, obs, config::max_depth, config::sample_l));

	clock_t clkStart;
	clock_t clkFinish;
	cout << "Start render" << endl;
	clkStart = clock();
	Image img = raytrace(config::threadpool_size, config::fnme, config::formula,
	                     sc, config::sample_n);
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
	cout << config::fnme << endl;
#elif false
	std::ifstream is("Extended_Cube.obj");
	TriangleMesh mesh(is, nullptr);
	cout << "triangles: " << mesh.triangles.size() << endl;
	cout << "leaves: " << mesh.leaves.size() << endl;
	cout << "hierarchy: " << mesh.hierarchy.size() << endl;
	for (auto hier : mesh.hierarchy) {
		cout << "{" << hier.left << " " << hier.right << " " << hier.leaves_i
		     << " " << hier.leaves_size << "}" << endl;
	}
#endif
};
