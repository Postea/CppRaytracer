
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "Config.h"
#include "material/Diffuse.h"
#include "material/Emitting.h"
#include "material/texture/Checkerboard.h"
#include "material/texture/Constant.h"
#include "sampling/Image.h"
#include "sampling/Scene.h"
#include "shape/Disk.h"
#include "shape/Group.h"
#include "shape/LightSingleGroup.h"
#include "shape/Rectangle.h"
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
std::array<size_t, 2> sample_configs[] = {
    {1, 3},
    {1, INT32_MAX},
};  // n, d

int main() {
	cout << "Start building scene" << endl;
	Mat4 ident;
	Group group(ident);
	auto checkered_board =
	    make_shared<Rectangle>(30.0f, 30.0f, false,
	                           make_shared<Diffuse>(make_shared<Checkerboard>(
	                               8, Vec3(0.7), Vec3(0.35))));
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
	// group.add(ShapeSingleGroup(rotate(Vec3(1, 0, 0), 90) *
	//                               rotate(Vec3(0, 1, 0), -90) *
	//                               translate(Vec3(8, 8, 0)),
	//                           checkered_board));
	// cube
	auto cube = make_shared<TriangleMesh>(std::ifstream("Cube.obj"),
	                                      make_shared<Diffuse>(Vec3(1, 0, 0)));
	group.add(ShapeSingleGroup(translate(Vec3(0, 1, -3)), cube));

	// light
	auto rect_light = make_shared<Rectangle>(30.0f, 30.0f, false,
	                                         make_shared<Emitting>(Vec3(1)));

	auto light_transform = rotate(Vec3(1, 0, 0), 90) *
	                       rotate(Vec3(0, 1, 0), -90) *
	                       translate(Vec3(8, 8, 0));

	group.add(ShapeSingleGroup(light_transform, rect_light));
	auto skysphere = make_shared<SkySphere>(make_shared<Constant>(Vec3(0)));
	group.add(skysphere);

	std::vector<std::shared_ptr<Light> > lights = {
	    make_shared<LightSingleGroup>(light_transform, rect_light)};
	cout << "Start rendering" << endl;

	for (const auto& [camera_key, camera] : config::cameras) {
		CamObs obs(camera, M_PI / 2, 350, 210);
		for (const auto& [sample_n, max_depth] : sample_configs) {
			//  scene building end
			auto sc = std::make_shared<Scene>(
			    Scene(group, lights, obs, max_depth, 0));

			auto fnme = config::file_name(sample_n, 0, max_depth, camera_key);
			clock_t clkStart;
			clock_t clkFinish;
			cout << "Start render" << endl;
			clkStart = clock();
			Image img = raytrace(threadpool_size, fnme, "", sc, sample_n);
			clkFinish = clock();
			cout << "Start imaging to " << fnme << endl;
			write_bmp(fnme.c_str(), img);
			cout << "End" << endl;
			std::cout << clkFinish - clkStart << endl;
		}
	}
};
