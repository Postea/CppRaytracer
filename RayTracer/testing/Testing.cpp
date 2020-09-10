#include "Testing.h"

#include <array>
#include <cassert>
#include <iostream>
#include <limits>

#include "../camera/Ray.h"
#include "../material/DiffuseMaterial.h"
#include "../shape/CirclePlane.h"
#include "../shape/Group.h"
#include "../shape/Sphere.h"
#include "../tools/AxisAlignedBoundingBox.h"
#include "../tools/Mat4.h"
#include "../tools/Vec3.h"

namespace test {
void vec3_test() {
	std::cout << "======================" << std::endl;
	std::cout << "     Testing Vec3    " << std::endl;
	std::cout << "======================" << std::endl;

	{
		// do not tolerate any memory overhead
		std::cout << "  sizeof(Vec3) == 3 bytes: ";
		assert(sizeof(util::Vec3) == 3 * sizeof(float));
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  constructor & index operator: ";
		util::Vec3 a;
		assert(a[0] == 0 && a[1] == 0 && a[2] == 0);
		util::Vec3 b(1, 2, 3);
		assert(b[0] == 1 && b[1] == 2 && b[2] == 3);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  read-only access to const object: ";
		const util::Vec3 a(1, 2, 3);
		// the next line will throw a compiler error if there is no proper
		// "operator[] const"
		assert(a[1] == 2);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  write access to a non-const object: ";
		util::Vec3 a(1, 2, 3);
		a[1] = 4;
		assert(a[0] == 1 && a[1] == 4 && a[2] == 3);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  comparison: ";
		util::Vec3 a(1, 2, 3), b(1, 2, 3), c(1, 2, 9), d(4, 5, 6);
		assert(a == b);
		assert(a == a);
		assert(a != c);
		assert(b != d);
		assert(!(a != b));
		std::cout << "passed." << std::endl;
	}
	{
		// should work out of the box when using std container (!)
		std::cout << "  assignment: ";
		util::Vec3 a(1, 2, 3);
		std::cout << a[0]
		          << " ";  // to make sure these values are not optimized away!
		a = util::Vec3(4, 5, 6);
		assert(a[0] == 4 && a[1] == 5 && a[2] == 6);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  addition: ";
		util::Vec3 a(1, 2, 3), b(4, 5, 6);
		a += b;
		assert(a == util::Vec3(5, 7, 9));
		auto c = a + util::Vec3(1, 1, 1);
		assert(c == util::Vec3(6, 8, 10));
		util::Vec3 one(1, 1, 1), four(4, 4, 4);
		one += one + one + one;
		assert(one == four);
		util::Vec3 x(0, 0, 0), y(1, 1, 1), z(2, 2, 2);
		y += z;
		assert(y == util::Vec3(3, 3, 3));
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  unary minus: ";
		util::Vec3 a(1, 2, 3);
		assert(-a == util::Vec3(-1, -2, -3));
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  dot product: ";
		util::Vec3 a(1, 2, 3);
		assert(dot(a, a) == 1 * 1 + 2 * 2 + 3 * 3);
		std::cout << "passed." << std::endl;
	}
	{
		// these tests will not compile if you forgot to declare
		// some methods const
		std::cout << "  constness: ";
		const util::Vec3 a(1, 2, 3);
		assert(a[1] == 2);
		assert(a == a);
		assert(!(a != a));
		assert(a + a == util::Vec3(2, 4, 6));
		assert(-a == util::Vec3(-1, -2, -3));
		assert(dot(a, a) == 1 * 1 + 2 * 2 + 3 * 3);
		std::cout << "passed." << std::endl;
	}
	{
		auto round_to_6 = [](float x) {
			int temp = x * 1000000;
			return temp / 1000000.0;
		};
		std::cout << "  length and normalize: ";
		const util::Vec3 a(1, 2, 3);
		assert(a.length() == (float)sqrt(14));
		assert(a.normalize()[0] == (float)(1 / sqrt(14)));
		util::Vec3 b(2, 2, 1);
		assert(b.length() == 3);
		assert(b.normalize()[1] == (float)(2.0 / 3.0));

		std::cout << "passed." << std::endl;
	}

	std::cout << "all util::Vec3 tests passed." << std::endl << std::endl;
}
void mat4_test() {
	std::cout << "======================" << std::endl;
	std::cout << "     Testing Mat4    " << std::endl;
	std::cout << "======================" << std::endl;

	{
		// do not tolerate any memory overhead
		std::cout << "  sizeof(Mat4) == 16 bytes: ";
		assert(sizeof(util::Mat4) == 16 * sizeof(float));
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  constructor & index operator: ";
		util::Mat4 a;
		assert(a == util::identity());
		util::Mat4 b({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
		assert((b[{0, 0}]) == 1 && (b[{1, 0}]) == 5 && (b[{3, 3}]) == 16);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  read-only access to const object: ";
		const util::Mat4 a(
		    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
		// the next line will throw a compiler error if there is no proper
		// "operator[] const"
		assert((a[{0, 2}]) == 3);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  write access to a non-const object: ";
		util::Mat4 a({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
		a[{0, 2}] = 4;
		// assert (a (0, 0) == 1 && a (0, 2) == 4 && a (2, 3) == 12);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  comparison: ";
		util::Mat4 a({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}),
		    b({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}),
		    c({1, 2, 9, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}),
		    d({4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19});
		assert(a == b);
		assert(a == a);
		assert(a != c);
		assert(b != d);
		assert(!(a != b));
		std::cout << "passed." << std::endl;
	}
	{
		// should work out of the box when using std container (!)
		std::cout << "  assignment: ";
		util::Mat4 a({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
		std::cout << a[{0, 0}]
		          << " ";  // to make sure these values are not optimized away!
		a = util::Mat4(
		    {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19});
		assert((a[{0, 0}]) == 4 && (a[{0, 1}]) == 5 && (a[{0, 2}]) == 6 &&
		       (a[{1, 2}]) == 10);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  multiplication: ";
		util::Mat4 a({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}),
		    b({4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19});
		util::Mat4 c = a * b;
		assert(c == util::Mat4({174, 196, 218, 240, 286, 324, 362, 400, 398,
		                        452, 506, 560, 510, 580, 650, 720}));
		auto d = a * util::identity();
		assert(d == a);
		util::Mat4 one({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		    four({4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4});
		one = one * one;
		assert(one == four);
		util::Mat4 x({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		    y({2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2});
		util::Mat4 z = x * y;
		assert(z ==
		       util::Mat4({8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}));
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  transpose: ";
		util::Mat4 a({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
		util::Mat4 b = a.transpose();
		assert(b == util::Mat4({1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8,
		                        12, 16}));
		util::Mat4 c({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
		assert(c == c.transpose());
		assert(util::identity() == util::identity().transpose());
		std::cout << "passed." << std::endl;
	}
	{
		// these tests will not compile if you forgot to declare
		// some methods const
		std::cout << "  constness: ";
		const util::Mat4 a(
		    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
		assert((a[{0, 1}]) == 2);
		assert(a == a);
		assert(!(a != a));
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  factory methods: ";
		util::Mat4 s = util::scale(util::Vec3(2, 3, 4));
		assert((s[{0, 0}]) == 2 && (s[{1, 1}]) == 3 && (s[{2, 2}]) == 4);
		util::Mat4 t = util::translate(util::Vec3(1, -3, 4));
		assert((t[{0, 3}]) == 1 && (t[{1, 3}]) == -3 && (t[{2, 3}]) == 4);
		auto round_to_6 = [](float x) {
			int temp = x * 1000000;
			return temp / 1000000.0;
		};
		util::Mat4 r = util::rotate(util::Vec3(1, 0, 0), 40);
		float r11 = round_to_6(r[{1, 1}]);
		float r12 = round_to_6(r[{1, 2}]);
		float r21 = round_to_6(r[{2, 1}]);
		float r22 = round_to_6(r[{2, 2}]);
		assert((r[{0, 0}]) == 1 && r11 == (float)0.766044 &&
		       r12 == (float)-0.642787 && r21 == (float)0.642787 &&
		       r22 == (float)0.766044);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  transform: ";
		auto round_to_5 = [](float x) {
			int temp = x * 100000;
			return temp / 100000.0;
		};
		util::Vec3 x = util::Vec3(2, 4, -5);
		util::Mat4 rota = util::rotate(util::Vec3(1, 0, 0), 40);
		util::Mat4 tran = util::translate(util::Vec3(1, 1, 1));
		util::Vec3 y = rota.transformPoint(x);
		assert(round_to_5(y[1]) == 6.27811 && round_to_5(y[2]) == -1.25907);
		y = tran.transformPoint(x);
		assert(round_to_5(y[1]) == 5 && round_to_5(y[2]) == -4);
		y = tran.transformDir(x);
		assert(round_to_5(y[1]) == 4 && round_to_5(y[2]) == -5);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  position: ";
		util::Mat4 x({0, 0, 0, 3, 0, 0, 1, 5, 1, 2, 3, -4, 2, 2, 2, 2});
		util::Vec3 vec = x.position();
		assert(vec[0] == 3 && vec[1] == 5 && vec[2] == -4);

		std::cout << "passed." << std::endl;
	}

	std::cout << "all util::Mat4 tests passed." << std::endl << std::endl;
}
void ray_test() {
	std::cout << "======================" << std::endl;
	std::cout << "     Testing Ray    " << std::endl;
	std::cout << "======================" << std::endl;

	{
		// do not tolerate any memory overhead
		std::cout << "  sizeof(Ray) == 36 bytes: ";
		assert(sizeof(cam::Ray) == 36);
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  point_at and borders: ";
		cam::Ray x(util::Vec3(1, 1, 1), util::Vec3(2, 0, 1), 0, 5000, false);
		assert(x(1) == util::Vec3(3, 1, 2));
		assert(x.in_range(400) && !x.in_range(-1) && !x.in_range(50000));
		cam::Ray y(util::Vec3(2, 4, -3), util::Vec3(4, -3, 1), 0, 5000, false);
		assert(y(5) == util::Vec3(22, -11, 2));
		std::cout << "passed." << std::endl;
	}
	{
		auto round_to_4 = [](float x) {
			int temp = x * 10000;
			return temp / 10000.0;
		};
		std::cout << "  normalize: ";
		cam::Ray x(util::Vec3(1, 1, 1), util::Vec3(0, 0, 5), 0, 5000, true);
		assert(x(1) == util::Vec3(1, 1, 2));
		cam::Ray y(util::Vec3(0, 0, 0), util::Vec3(3, 3, 3), 0, 5000, true);
		assert(round_to_4(y(2)[0]) == 1.1547);
		std::cout << "passed." << std::endl;
	}
	std::cout << "all cam::Ray tests passed." << std::endl << std::endl;
}
void axisalignedboundingbox_test() {
	std::cout << "======================" << std::endl;
	std::cout << "     Testing AABB    " << std::endl;
	std::cout << "======================" << std::endl;

	{
		// do not tolerate any memory overhead
		std::cout << "  sizeof(AABB) == 24 bytes: ";
		assert(sizeof(util::AxisAlignedBoundingBox));
		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  contains: ";
		util::AxisAlignedBoundingBox bb(util::Vec3(10, 10, 10),
		                                util::Vec3(20, 20, 20));
		assert(!bb.contains(util::Vec3(10, 6, 13)));
		assert(!bb.contains(util::Vec3(22, 15, 13)));
		assert(bb.contains(util::Vec3(10, 10, 20)));
		assert(!bb.contains(util::Vec3(9, 19, 20)));
		assert(!bb.contains(util::Vec3(16, 13, 25)));
		assert(bb.contains(util::Vec3(15, 10, 18)));
		assert(bb.contains(util::Vec3(10, 15, 20)));
		assert(bb.contains(util::Vec3(10, 10, 10)));

		// 2-dimensional AABB contain test
		util::AxisAlignedBoundingBox bb0(util::Vec3(0, 10, 10),
		                                 util::Vec3(0, 20, 20));
		assert(!bb0.contains(util::Vec3(10, 6, 13)));
		assert(!bb0.contains(util::Vec3(22, 15, 13)));
		assert(bb0.contains(util::Vec3(0, 10, 20)));
		assert(bb0.contains(util::Vec3(0, 19, 20)));
		assert(!bb0.contains(util::Vec3(0, 13, 25)));
		assert(bb0.contains(util::Vec3(0, 10, 18)));
		assert(!bb0.contains(util::Vec3(10, 15, 20)));
		assert(bb0.contains(util::Vec3(0, 10, 10)));

		// Infinity contain test
		util::AxisAlignedBoundingBox infbb;
		assert(infbb.contains(util::Vec3(1243, 1341, -3151)));
		assert(infbb.contains(
		    util::Vec3(-std::numeric_limits<float>::infinity())));
		assert(
		    infbb.contains(util::Vec3(std::numeric_limits<float>::infinity())));
		assert(infbb.contains(util::Vec3(0)));

		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  expand: ";
		util::AxisAlignedBoundingBox bb(util::Vec3(10, 10, 10),
		                                util::Vec3(20, 20, 20));

		util::AxisAlignedBoundingBox bb2(util::Vec3(15, 15, 15),
		                                 util::Vec3(25, 25, 25));

		auto result = bb + bb2;
		assert(result.minBound() == util::Vec3(10, 10, 10));
		assert(result.maxBound() == util::Vec3(25, 25, 25));

		util::AxisAlignedBoundingBox bb3(util::Vec3(10, 10, 10),
		                                 util::Vec3(20, 20, 20));

		util::AxisAlignedBoundingBox bb4(util::Vec3(15, 9, -1),
		                                 util::Vec3(18, 10, 25));
		auto result2 = bb3 + bb4;
		assert(result2.minBound() == util::Vec3(10, 9, -1));
		assert(result2.maxBound() == util::Vec3(20, 20, 25));

		util::AxisAlignedBoundingBox bb5(util::Vec3(-45, 40, 0),
		                                 util::Vec3(0, 40, 20));

		util::AxisAlignedBoundingBox bb6(util::Vec3(-70, -5, -1),
		                                 util::Vec3(-60, 90, 0));
		auto result3 = bb5 + bb6;

		assert(result3.minBound() == util::Vec3(-70, -5, -1));
		assert(result3.maxBound() == util::Vec3(0, 90, 20));

		// Infinity expanse test
		util::AxisAlignedBoundingBox infbb;
		auto result4 = infbb + result3 + result2 + result;
		assert(result4.maxBound() == infbb.maxBound() &&
		       result4.minBound() == infbb.minBound());

		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  intersect: ";
		util::AxisAlignedBoundingBox bb(util::Vec3(10, 10, 10),
		                                util::Vec3(20, 20, 20));

		assert(bb.intersects(cam::Ray(util::Vec3(1, 10, 10),
		                              util::Vec3(1, 0.5, 0.3), 0, 100, false)));
		assert(bb.intersects(cam::Ray(
		    util::Vec3(11, 11, 11), util::Vec3(0.5, 0.3, 0.5), 0, 100, false)));
		assert(!bb.intersects(cam::Ray(
		    util::Vec3(9, 9, 9), util::Vec3(1, -0.4, -0.5), 0, 100, false)));
		assert(!bb.intersects(cam::Ray(
		    util::Vec3(21, 21, 21), util::Vec3(1, -0.4, -0.5), 0, 100, false)));
		assert(
		    bb.intersects(cam::Ray(util::Vec3(21, 21, 21),
		                           util::Vec3(-1, -0.4, -0.5), 0, 100, false)));
		assert(bb.intersects(cam::Ray(util::Vec3(21, 21, 21),
		                              util::Vec3(-1, -1, -2), 0, 100, false)));

		// 2-dimensional AABB intersect test
		util::AxisAlignedBoundingBox bb0(util::Vec3(0, 10, 10),
		                                 util::Vec3(0, 20, 20));
		assert(bb0.intersects(cam::Ray(util::Vec3(21, 10, 11),
		                               util::Vec3(-1, 0, 0), 0, 100, false)));
		assert(bb0.intersects(cam::Ray(util::Vec3(-1, 10, 9),
		                               util::Vec3(1, 0, 2), 0, 100, false)));
		assert(!bb0.intersects(cam::Ray(
		    util::Vec3(0, 21, 21), util::Vec3(-1, -2, -2), 0, 100, false)));
		assert(bb0.intersects(cam::Ray(util::Vec3(-1, 18, 18),
		                               util::Vec3(1, -2, 2), 0, 100, false)));

		// Infinity intersection test
		util::AxisAlignedBoundingBox infbb;
		assert(infbb.intersects(cam::Ray(
		    util::Vec3(21341, -13421, 0), util::Vec3(-1315, -11324, 2135), 0,
		    std::numeric_limits<float>::infinity(), false)));

		std::cout << "passed." << std::endl;
	}
	std::cout << "all util::AxisAlignedBoundingBox tests passed." << std::endl
	          << std::endl;
}

void shape_test() {
	std::cout << "======================" << std::endl;
	std::cout << "     Testing Shapes   " << std::endl;
	std::cout << "======================" << std::endl;

	auto red_material =
	    std::make_shared<material::DiffuseMaterial>(util::Vec3(1, 0, 0));
	{
		std::cout << "  CirclePlane: ";

		shapes::CirclePlane circ_plane(5.0, red_material);
		cam::Ray direct_ray(util::Vec3(0, 3, 0), util::Vec3(0.3, -1, 0.2), 0,
		                    100, false);
		cam::Ray bounding_ray(util::Vec3(4.6, -1, 4), util::Vec3(-0.1, 1, 0.5),
		                      0, 100, false);
		cam::Ray missing_ray(util::Vec3(4.6, -1, 4), util::Vec3(-0.1, -1, 0.5),
		                     0, 100, false);

		assert(circ_plane.bounds().intersects(direct_ray));
		assert(circ_plane.bounds().intersects(bounding_ray));
		assert(!circ_plane.bounds().intersects(missing_ray));

		assert(circ_plane.intersect(direct_ray));
		assert(!circ_plane.intersect(bounding_ray));
		assert(!circ_plane.intersect(missing_ray));

		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  Sphere: ";

		shapes::Sphere sphere(5.0, red_material);
		cam::Ray direct_ray(util::Vec3(3, 6, 3), util::Vec3(-0.3, -2, 0.2), 0,
		                    100, false);
		cam::Ray bounding_ray(util::Vec3(4.5, 5.3, 4),
		                      util::Vec3(-0.5, -0.5, 0.4), 0, 100, false);
		cam::Ray missing_ray(util::Vec3(5.5, 5.5, 5.5), util::Vec3(1, 1, 1), 0,
		                     100, false);

		assert(sphere.bounds().intersects(direct_ray));
		assert(sphere.bounds().intersects(bounding_ray));
		assert(!sphere.bounds().intersects(missing_ray));

		assert(sphere.intersect(direct_ray));
		assert(!sphere.intersect(bounding_ray));
		assert(!sphere.intersect(missing_ray));

		std::cout << "passed." << std::endl;
	}
	{
		std::cout << "  Transform intersect: ";

		auto matrix = util::translate(util::Vec3(-2, 0, 0));
		auto transform = util::Transformation(matrix);
		auto group = shapes::shapeGroup(
		    matrix, std::make_shared<shapes::Sphere>(0.5, red_material));
		cam::Ray direct_ray(util::Vec3(-2, 0, 0), util::Vec3(0.4, 0, 0), 0, 100,
		                    false);
		cam::Ray bounding_ray(util::Vec3(4.5, 5.3, 4),
		                      util::Vec3(-0.5, -0.5, 0.4), 0, 100, false);
		cam::Ray missing_ray(util::Vec3(5.5, 5.5, 5.5), util::Vec3(1, 1, 1), 0,
		                     100, false);
		std::cout << "Direct Ray: " << direct_ray << std::endl;
		std::cout << "Transposed direct Ray: "
		          << cam::Ray(transform.fromWorld.transformPoint(direct_ray.x0),
		                      transform.fromWorld.transformDir(direct_ray.d),
		                      direct_ray.tmin, direct_ray.tmax,
		                      direct_ray.normalize);
		assert(group.intersect(direct_ray));
	}
	std::cout << "all shapes::Shape tests passed." << std::endl;
}
}  // namespace test
