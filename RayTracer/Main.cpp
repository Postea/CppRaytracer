
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "material/BackgroundMaterial.h"
#include "material/DiffuseMaterial.h"
#include "material/ReflectiveMaterial.h"
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
	if (true) {
		cout << "Start" << endl;
		// Image img = Image (100, 100);

		Mat4 ident;
#if true
		Mat4 camm = rotate(Vec3(1, 0, 0), -30) * rotate(Vec3(0, 1, 0), -45) *
		            translate(Vec3(0, 3, 2));
#else
		Mat4 camm = rotate(Vec3(1, 0, 0), -90) * rotate(Vec3(1, 0, 0), 0) *
		            translate(Vec3(0, 3, 0));
#endif
		CamObs obs(camm, M_PI / 2, 600, 360);

		Group group(ident);

		Image sky_image = readImage("textures/moonlit_golf_4k.hdr");
		auto skysphere = make_shared<SkySphere>(
		    SkySphere(make_shared<Image>(sky_image), sky_image));

		group.add(skysphere);

		auto floor = make_shared<RectanglePlane>(
		    1000.0f, 1000.0f, false, make_shared<DiffuseMaterial>(Vec3(0.9)));

		group.add(ShapeSingleGroup(translate(Vec3(0, -1, 0)), floor));

		std::ifstream roo("Tower_Base.obj");
		TriangleMesh rook(roo,
		                  make_shared<DiffuseMaterial>(Vec3(0.1f, 0.9f, 0.1f)));
		group.add(ShapeSingleGroup(translate(Vec3(6, 0, -2)),
		                           make_shared<TriangleMesh>(rook)));

		auto red_sphere = make_shared<Sphere>(
		    1.0f, make_shared<DiffuseMaterial>(Vec3(0.9f, 0.1f, 0.1f)));
		group.add(ShapeSingleGroup(translate(Vec3(2, 0, -2)), red_sphere));

		auto green_sphere = make_shared<Sphere>(
		    1.0f, make_shared<DiffuseMaterial>(Vec3(0.1f, 0.9f, 0.1f)));
		// group.add(ShapeSingleGroup(translate(Vec3(4, 0, -2)), green_sphere));

		std::vector<std::shared_ptr<Light>> lights = {skysphere};
		auto sc = std::make_shared<Scene>(Scene(group, lights, obs, 8));

		size_t n = 1;

		clock_t clkStart;
		clock_t clkFinish;
		cout << "Start render" << endl;
		clkStart = clock();
		Image img = raytrace(4, sc, n * n);
		clkFinish = clock();
		cout << "Start imaging" << endl;
		writeBmp("results/aaa.bmp", img);
		cout << "End" << endl;
		std::cout << clkFinish - clkStart;

	} else if (false) {
		// test::vec3_test();
		// test::mat4_test();
		// test::ray_test();
		// test::shape_test();
		test::axisalignedboundingbox_test();
	} else {
		std::ifstream is("Extended_Cube.obj");
		TriangleMesh mesh(is, nullptr);
		cout << "triangles: " << mesh.triangles.size() << endl;
		cout << "leaves: " << mesh.leaves.size() << endl;
		cout << "hierarchy: " << mesh.hierarchy.size() << endl;
		for (auto hier : mesh.hierarchy) {
			cout << "{" << hier.left << " " << hier.right << " "
			     << hier.leaves_i << " " << hier.leaves_size << "}" << endl;
		}
	}
};
