#include "Mat4.h"
#include <iostream>
#include "Testing.h"
#include "Ray.h"
#include "Constant.h"
#include <math.h>
#include "Image.h"
#include <fstream>
#include "Scene.h"
#include "Background.h"
#include "BackgroundMaterial.h"
#include "CirclePlane.h"
#include "DiffuseMaterial.h"

using namespace material;
using namespace util;
using namespace test;
using namespace cam;
using namespace shapes;
using namespace std;

void writeBmp (const char* filename, Image img) {

	int width = img.width_;   // Width of the image
	int height = img.height_; // Height of the image

	int horizontalBytes;      // Horizontal bytes to add, so the pixelarrays width is a multiple of 4
	horizontalBytes = 4 - ((width * 3) % 4);
	if (horizontalBytes == 4)
		horizontalBytes = 0;

	int pixelArraySize;		  // The size of the pixelArray
	pixelArraySize = ((width * 3) + horizontalBytes) * height;

	// Headers
	int header[12];
	// Bitmap file header
	char bb = 'B';					 // bfType		
	char mm = 'M';
	header[0] = pixelArraySize + 54; // bfSize
	header[1] = 0;                   // bfReserved
	header[2] = 54;                  // bfOffbits
	// Bitmap information header
	header[3] = 40;                  // biSize
	header[4] = width;				 // biWidth
	header[5] = height;				 // biHeight
	short biPLanes = 1;				 // biPlanes
	short biBitCount = 24;			 // biBitCount
	header[6] = 0;                   // biCompression
	header[7] = pixelArraySize;      // biSizeImage
	header[8] = 0;                   // biXPelsPerMeter
	header[9] = 0;                   // biYPelsPerMeter
	header[10] = 0;                  // biClrUsed
	header[11] = 0;					 // biClrImportant

	ofstream ofile (filename, std::ios::binary);

	// Write the header in the right order
	// bfType, ...
	ofile.write (&bb, sizeof (bb));
	ofile.write (&mm, sizeof (mm));
	// ... bfSize, BfReserved, bfOffbits, biSize, biWidth, bitHeight, ...
	for (int i = 0; i < 6; i++) {
		ofile.write ((char*)& header[i], sizeof (header[i]));
	}
	// ... biPlanes, bitBitCount, ...
	ofile.write ((char*)& biPLanes, sizeof (biPLanes));
	ofile.write ((char*)& biBitCount, sizeof (biBitCount));
	// ... biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant
	for (int i = 6; i < 12; i++) {
		ofile.write ((char*)& header[i], sizeof (header[i]));
	}
	// The colors can only have a value from 0 to 255, so a char is enough to store them
	char blue, green, red;
	// Bmp is written from top to bottom
	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x <= width - 1; x++)
		{
			red = ((img[{x, y}][0]) * 255);
			green = ((img[{x, y}][1]) * 255);
			blue = ((img[{x, y}][2]) * 255);

			//bmp colors are bgr not rgb
			char bgr[3] = { blue, green , red };
			for (int i = 0; i < 3; i++) {
				char c0 = (bgr[i] & 0x00FF);
				//char c8 = ((bgr[i] & (unsigned int)0xFF00) >> 8);

				ofile.write (&c0, sizeof (c0));
				//ofile.write (&c8, sizeof (c8));

			}
		}
		// If needed add extra bytes after each row
		if (horizontalBytes != 0)
		{
			char null;
			for (int n = 0; n < horizontalBytes; n++)
			{
				ofile.write (&null, sizeof (null));
			}
		}
	}

	ofile.close ();
}
void printBits (void const* const ptr, size_t const size)
{
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			printf ("%u", byte);
		}
		printf ("\n");
	}
	puts ("");
}

int main ()
{
	//Image img = Image (100, 100);

	Mat4 ident;
	Mat4 camm = translate (Vec3 (0, 1, 3));
	//Mat4 group22Mat = rotate (Vec3 (1, 0, 0), 0);
	CamObs obs (camm, M_PI / 2, 500, 500);
	shared_ptr<Group> group = make_shared<Group> (ident);
	shared_ptr<Group> group2 = make_shared<Group> (ident);

	BackgroundMaterial bg_colo (Vec3 (1, 1, 1));
	Background bg (make_shared<BackgroundMaterial> (bg_colo));

	DiffuseMaterial circ_colo (Vec3 (0, 0, 0.5));
	CirclePlane circ (10, make_shared < DiffuseMaterial > (circ_colo));

	group->add (make_shared<Background> (bg));
	group->add (group2);
	group2->add (make_shared<CirclePlane> (circ));

	auto sc = std::make_shared<Scene>(Scene  (group, obs, 2));

	size_t n = 1;
	Image img = raytrace (obs,sc, n*n);
	//img.setPixels (std::make_shared<Scene> (sc));

	writeBmp ("C:/Users/ydb19/Desktop/test.bmp", img);
};
