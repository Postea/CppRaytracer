
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
    {1, 1}, {1, 6}, {1, 7}, {2, 0}, {2, 16}, {5, 1}, {6, 0}, {10, 6}, {20, 0},
};  // n, l
size_t max_depth = 8;
auto sky_key = "FullMoon";

int main() {
	cout << "Start" << endl;

	Mat4 ident;

	Group group(ident);

	auto floor = make_shared<Rectangle>(1000.0f, 1000.0f, false,
	                                    make_shared<Diffuse>(Vec3(0.3)));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1.001, 0)), floor));

	auto checkered_board = make_shared<Rectangle>(
	    16.0f, 16.0f, false,
	    make_shared<Diffuse>(make_shared<Checkerboard>(8, Vec3(1), Vec3(0.6))));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1, 0)), checkered_board));

	TriangleMesh rook(std::ifstream("Tower_Base.obj"),
	                  make_shared<Diffuse>(Vec3(0.0f, 0.0f, 1.0f)));
	TriangleMesh cube(std::ifstream("Cube.obj"),
	                  make_shared<Diffuse>(Vec3(0.0f, 1.0f, 0.0f)));

	group.add(ShapeSingleGroup(translate(Vec3(0, 0, 0)),
	                           make_shared<TriangleMesh>(rook)));
	group.add(ShapeSingleGroup(translate(Vec3(3, 0, 0)),
	                           make_shared<TriangleMesh>(cube)));

	auto red_sphere =
	    make_shared<Sphere>(1.0f, make_shared<Diffuse>(Vec3(1.0f, 0.0f, 0.0f)));
	group.add(ShapeSingleGroup(translate(Vec3(-3, 1, 0)), red_sphere));
	// Sky texture
	Image sky_image = read_image(config::skies[sky_key].c_str());
	sky_image.half_image(true, 0.01);
	auto skysphere = make_shared<SkySphere>(
	    SkySphere(make_shared<Image>(sky_image), sky_image, 10.0f));
	group.add(skysphere);

	std::vector<std::shared_ptr<Light>> lights = {skysphere};

	cout << "Start rendering" << endl;
	for (const auto& [camera_key, camera] : config::cameras) {
		CamObs obs(camera, M_PI / 2, 350, 210);
		for (const auto& [sample_n, sample_l] : sample_configs) {
			auto sc = std::make_shared<Scene>(
			    Scene(group, lights, obs, max_depth, sample_l));

			auto fnme = config::file_name(sample_n, sample_l, max_depth,
			                              camera_key, sky_key);
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
