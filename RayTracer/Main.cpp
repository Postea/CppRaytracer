
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
#include "shape/Background.h"
#include "shape/CirclePlane.h"
#include "shape/Group.h"
#include "shape/LightSingleGroup.h"
#include "shape/RectanglePlane.h"
#include "shape/ShapeSingleGroup.h"
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
		Mat4 camm = rotate(Vec3(1, 0, 0), -45) * rotate(Vec3(1, 0, 0), 0) *
		            translate(Vec3(0, 3, 3));
#else
		Mat4 camm = rotate(Vec3(1, 0, 0), -90) * rotate(Vec3(1, 0, 0), 0) *
		            translate(Vec3(0, 3, 0));
#endif
		CamObs obs(camm, M_PI / 2, 500, 500);

		auto bg_colo = make_shared<BackgroundMaterial>(Vec3(0.1));
		auto light_colo =
		    make_shared<DiffuseMaterial>(make_shared<Constant>(Vec3(1, 0, 0)),
		                                 make_shared<Constant>(Vec3(1, 0, 0)));
		auto circ_colo = make_shared<DiffuseMaterial>(Vec3(0.3));
		auto sphere_colo = make_shared<DiffuseMaterial>(Vec3(0.5));
		auto mirror_colo = make_shared<ReflectiveMaterial>(Vec3(0.3), (float)0);

		Group group(ident);
		Group shape_group(ident);
		auto light = make_shared<CirclePlane>((float)0.5, false, light_colo);
		auto light_matrix =
		    rotate(Vec3(0, 0, 1), -90) * translate(Vec3(-1, 0, 0));
		auto lightGroup = make_shared<LightSingleGroup>(light_matrix, light);

		auto circ = ShapeSingleGroup(
		    translate(Vec3(0, -1, 2)),
		    make_shared<RectanglePlane>((float)4, (float)2, false, circ_colo));
		auto sphere =
		    ShapeSingleGroup(translate(Vec3(0, 0, 0)),
		                     make_shared<Sphere>((float)0.5, sphere_colo));
		// shape_group.add(circ);
		// shape_group.add(sphere);
		shape_group.add(ShapeSingleGroup(light_matrix, light));

		std::ifstream is("Tower_Base.obj");
		auto mesh = make_shared<TriangleMesh>(is, sphere_colo);

		cout << "triangles: " << mesh->triangles.size() << endl;
		cout << "leaves: " << mesh->leaves.size() << endl;
		cout << "hierarchy: " << mesh->hierarchy.size() << endl;

		float s = 0;
		for (auto hier : mesh->hierarchy) {
			cout << "{" << hier.left << " " << hier.right << " "
			     << hier.leaves_i << " " << hier.leaves_size << " "
			     << hier.bb.size() << "}" << endl;
			s += hier.bb.size();
		}
		cout << "Size: " << s << endl;

		shape_group.add(ShapeSingleGroup(ident, mesh));
		/*for (auto pos : mesh->vertices) {
		        std::cout << pos.position << std::endl;
		        shape_group.add(
		            ShapeSingleGroup(translate(pos.position),
		                             make_shared<Sphere>(0.2, sphere_colo)));
		    }
		    */
		group.add(shape_group);
		group.add(make_shared<Background>(bg_colo));

		std::vector<std::shared_ptr<Light>> lights = {lightGroup};
		auto sc = std::make_shared<Scene>(Scene(group, lights, obs, 8));

		size_t n = 1;

		clock_t clkStart;
		clock_t clkFinish;
		cout << "Start render" << endl;
		clkStart = clock();
		Image img = raytrace(4, obs, sc, n * n);
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
		std::ifstream is("Tower_Top.obj");
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
