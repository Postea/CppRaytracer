
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#include <fstream>
#include <iostream>

#include "material/BackgroundMaterial.h"
#include "material/DiffuseMaterial.h"
#include "sampling/Image.h"
#include "sampling/Scene.h"
#include "shape/Background.h"
#include "shape/CirclePlane.h"
#include "shape/Group.h"
#include "shape/Sphere.h"
#include "testing/Testing.h"
#include "tools/Mat4.h"

using namespace material;
using namespace util;
using namespace test;
using namespace cam;
using namespace shapes;
using namespace std;

void writeBmp(const char* filename, Image img) {
	int width = img.width;    // Width of the image
	int height = img.height;  // Height of the image

	int horizontalBytes;  // Horizontal bytes to add, so the pixelarrays width
	                      // is a multiple of 4
	horizontalBytes = 4 - ((width * 3) % 4);
	if (horizontalBytes == 4) horizontalBytes = 0;

	int pixelArraySize;  // The size of the pixelArray
	pixelArraySize = ((width * 3) + horizontalBytes) * height;

	// Headers
	int header[12];
	// Bitmap file header
	char bb = 'B';  // bfType
	char mm = 'M';
	header[0] = pixelArraySize + 54;  // bfSize
	header[1] = 0;                    // bfReserved
	header[2] = 54;                   // bfOffbits
	// Bitmap information header
	header[3] = 40;              // biSize
	header[4] = width;           // biWidth
	header[5] = height;          // biHeight
	short biPLanes = 1;          // biPlanes
	short biBitCount = 24;       // biBitCount
	header[6] = 0;               // biCompression
	header[7] = pixelArraySize;  // biSizeImage
	header[8] = 0;               // biXPelsPerMeter
	header[9] = 0;               // biYPelsPerMeter
	header[10] = 0;              // biClrUsed
	header[11] = 0;              // biClrImportant

	ofstream ofile(filename, std::ios::binary);

	// Write the header in the right order
	// bfType, ...
	ofile.write(&bb, sizeof(bb));
	ofile.write(&mm, sizeof(mm));
	// ... bfSize, BfReserved, bfOffbits, biSize, biWidth, bitHeight, ...
	for (int i = 0; i < 6; i++) {
		ofile.write((char*)&header[i], sizeof(header[i]));
	}
	// ... biPlanes, bitBitCount, ...
	ofile.write((char*)&biPLanes, sizeof(biPLanes));
	ofile.write((char*)&biBitCount, sizeof(biBitCount));
	// ... biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter,
	// biClrUsed, biClrImportant
	for (int i = 6; i < 12; i++) {
		ofile.write((char*)&header[i], sizeof(header[i]));
	}
	// The colors can only have a value from 0 to 255, so a char is enough to
	// store them
	char blue, green, red;
	// Bmp is written from top to bottom
	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x <= width - 1; x++) {
			red = ((img[{x, y}][0]) * 255);
			green = ((img[{x, y}][1]) * 255);
			blue = ((img[{x, y}][2]) * 255);

			// bmp colors are bgr not rgb
			char bgr[3] = {blue, green, red};
			for (int i = 0; i < 3; i++) {
				char c0 = (bgr[i] & 0x00FF);
				// char c8 = ((bgr[i] & (unsigned int)0xFF00) >> 8);

				ofile.write(&c0, sizeof(c0));
				// ofile.write (&c8, sizeof (c8));
			}
		}
		// If needed add extra bytes after each row
		if (horizontalBytes != 0) {
			char null;
			for (int n = 0; n < horizontalBytes; n++) {
				ofile.write(&null, sizeof(null));
			}
		}
	}

	ofile.close();
}
void printBits(void const* const ptr, size_t const size) {
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--) {
		for (j = 7; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
		printf("\n");
	}
	puts("");
}

void print_AABB(string x, shared_ptr<Shape> shape) {
	cout << x << shape->bounds().minBound() << shape->bounds().maxBound()
	     << endl;
}

int main() {
	cout << "Start" << endl;
	// Image img = Image (100, 100);

	Mat4 ident;
	Mat4 camm = translate(Vec3(0, 1, 3));
	// Mat4 group22Mat = rotate (Vec3 (1, 0, 0), 0);
	CamObs obs(camm, M_PI / 2, 500, 500);

	auto bg_colo = make_shared<BackgroundMaterial>(Vec3(1, 1, 1));
	auto circ_colo = make_shared<DiffuseMaterial>(Vec3(0, 1, 0.5));
	auto sphere_colo = make_shared<DiffuseMaterial>(Vec3(1, 0, 0));

	Group group(ident);
	Group shape_group(ident);

	auto background = shapeGroup(ident, make_shared<Background>(bg_colo));
	auto circ = shapeGroup(translate(Vec3(0, 2, 0)),
	                       make_shared<CirclePlane>((float)10, circ_colo));
	auto sphere = shapeGroup(translate(Vec3(0, 3, 0)),
	                         make_shared<Sphere>((float)0.5, sphere_colo));

	shape_group.add(sphere);
	shape_group.add(circ);
	print_AABB("ShapeGroup: ", make_shared<Group>(shape_group));
	print_AABB("Sphere: ", make_shared<Group>(sphere));

	group.add(shape_group);

	auto sc =
	    std::make_shared<Scene>(Scene(group, Background(bg_colo), obs, 3));

	size_t n = 4;

	clock_t clkStart;
	clock_t clkFinish;
	cout << "Start render" << endl;
	clkStart = clock();
	Image img = raytrace(4, obs, sc, n * n);
	clkFinish = clock();
	cout << "Start imaging" << endl;
	writeBmp("results/test.bmp", img);
	cout << "End" << endl;
	std::cout << clkFinish - clkStart;

	test::vec3_test();
	test::mat4_test();
	test::ray_test();
	test::shape_test();
};
