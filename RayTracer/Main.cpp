
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

size_t threadpool_size = 4;
auto s = {1};
auto l = {0};
auto d = {8};

int main() {
	cout << "Start building scene" << endl;
	Mat4 ident;
	Group group(ident);
	auto checkered_board = make_shared<RectanglePlane>(
	    30.0f, 30.0f, false,
	    make_shared<DiffuseMaterial>(
	        make_shared<Checkerboard>(8, Vec3(0.8), Vec3(0.4))));
	// floor
	group.add(ShapeSingleGroup(translate(Vec3(0, 0, 0)), checkered_board));
	// ceiling
	group.add(
	    ShapeSingleGroup(rotate(Vec3(1, 0, 0), 180) * translate(Vec3(0, 10, 0)),
	                     checkered_board));
	// back wall
	group.add(
	    ShapeSingleGroup(rotate(Vec3(1, 0, 0), 90) * translate(Vec3(0, 8, -5)),
	                     checkered_board));
	// front wall
	group.add(
	    ShapeSingleGroup(rotate(Vec3(1, 0, 0), -90) * translate(Vec3(0, 8, 8)),
	                     checkered_board));
	// left wall
	group.add(
	    ShapeSingleGroup(rotate(Vec3(1, 0, 0), 90) * rotate(Vec3(0, 1, 0), 90) *
	                         translate(Vec3(-8, 8, 0)),
	                     checkered_board));
	// right wall
	group.add(ShapeSingleGroup(rotate(Vec3(1, 0, 0), 90) *
	                               rotate(Vec3(0, 1, 0), -90) *
	                               translate(Vec3(8, 8, 0)),
	                           checkered_board));
	// cube
	auto cube = make_shared<TriangleMesh>(
	    std::ifstream("Cube.obj"), make_shared<DiffuseMaterial>(Vec3(1, 0, 0)));
	group.add(ShapeSingleGroup(translate(Vec3(0, 1, -3)), cube));

	// light
	auto rect_light = make_shared<RectanglePlane>(
	    1.0f, 1.0f, false, make_shared<BackgroundMaterial>(Vec3(1)));

	auto light_transform = rotate(Vec3(1, 0, 0), 90) *
	                       rotate(Vec3(0, 1, 0), -90) *
	                       translate(Vec3(7.99, 1.5, 0));

	group.add(ShapeSingleGroup(light_transform, rect_light));
	auto skysphere = make_shared<SkySphere>(make_shared<Constant>(Vec3(50)));
	group.add(skysphere);

	std::vector<std::shared_ptr<Light> > lights = {
	    make_shared<LightSingleGroup>(light_transform, rect_light)};
	cout << "Start rendering" << endl;

	for (const auto& [camera_key, camera] : config::cameras) {
		CamObs obs(camera, M_PI / 2, 600, 360);

		for (const auto& sample_n : s) {
			for (const auto& sample_l : l) {
				for (const auto& max_depth : d) {
					//  scene building end
					auto sc = std::make_shared<Scene>(
					    Scene(group, lights, obs, max_depth, sample_l));

					auto fnme = config::file_name(sample_n, sample_l, max_depth,
					                              camera_key);
					clock_t clkStart;
					clock_t clkFinish;
					cout << "Start render" << endl;
					clkStart = clock();
					Image img =
					    raytrace(threadpool_size, fnme, "", sc, sample_n);
					clkFinish = clock();
					cout << "Start imaging to " << fnme << endl;
					writeBmp(fnme.c_str(), img);
					cout << "End" << endl;
					std::cout << clkFinish - clkStart << endl;
				}
			}
		}
	}
};
