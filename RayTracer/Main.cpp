
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
	cout << "Start" << endl;
	// Image img = Image (100, 100);

	Mat4 ident;
	CamObs obs(config::camera, M_PI / 2, 600, 360);

	Group group(ident);

	Image sky_image = read_image(config::sky.c_str());
	sky_image.half_image(true, 0.01);
	auto skysphere =
	    make_shared<SkySphere>(make_shared<Image>(sky_image), sky_image, 2.0f);
	// auto skysphere =
	// make_shared<SkySphere>(make_shared<Constant>(Vec3(0.9)));

	group.add(skysphere);

	auto floor = make_shared<Rectangle>(1000.0f, 1000.0f, false,
	                                    make_shared<Diffuse>(Vec3(0.3)));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1.001, 0)), floor));

	auto checkered_board =
	    make_shared<Rectangle>(16.0f, 16.0f, false,
	                           make_shared<Diffuse>(make_shared<Checkerboard>(
	                               8, Vec3(0.8), Vec3(0.4))));

	group.add(ShapeSingleGroup(translate(Vec3(0, -1, 0)), checkered_board));

	TriangleMesh rook(std::ifstream("Tower_Base.obj"),
	                  make_shared<Diffuse>(Vec3(0.05f, 0.05f, 0.9f)));
	TriangleMesh cube(std::ifstream("Cube.obj"),
	                  make_shared<Diffuse>(Vec3(0.05f, 0.9f, 0.05f)));

	group.add(ShapeSingleGroup(translate(Vec3(0, 0, 0)),
	                           make_shared<TriangleMesh>(rook)));
	group.add(ShapeSingleGroup(translate(Vec3(3, 0, 0)),
	                           make_shared<TriangleMesh>(cube)));

	auto red_sphere = make_shared<Sphere>(
	    1.0f, make_shared<Diffuse>(Vec3(0.9f, 0.05f, 0.05f)));
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
	write_bmp(config::fnme.c_str(), img);
	cout << "End" << endl;
	std::cout << clkFinish - clkStart;
};