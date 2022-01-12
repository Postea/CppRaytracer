#include "Image.h"

#include <fstream>
#include <iostream>

#include "../tools/Threadpool.h"
#include "StratifiedSampler.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"

namespace util {
Image::Image(int width, int height)
    : width(width), height(height), halfed(false) {
	Vec3 color({});
	for (int i = 0; i < width * height; i++) {
		vec.insert(vec.end(), color);
	}
}
void Image::setPixel(int x, int y, Vec3 color) {
	vec[width * y + x] = color;
}
void Image::setPixels(size_t threadcount, std::shared_ptr<Sampler> sampler) {
	Threadpool tp(threadcount);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			tp.queueTask(std::bind([this, x, y, sampler]() {
				this->setPixelsTask(x, y, sampler);
			}));
		}
	}
}

void Image::setPixelsTask(int x, int y, std::shared_ptr<Sampler> sampler) {
	Vec3 v = sampler->color(x, y);
	setPixel(x, y, v);
}

void Image::gammaCorrect(float gamma) {
	// correct the whole data-array with the given gamma
	std::transform(vec.begin(), vec.end(), vec.begin(),
	               [gamma](util::Vec3 v) -> util::Vec3 {
		               return util::Vec3(std::powf(v.x(), 1 / gamma),
		                                 std::powf(v.y(), 1 / gamma),
		                                 std::powf(v.z(), 1 / gamma));
	               });
}
Vec3 Image::operator[](const std::array<int, 2>& i) const {
	return vec[width * i[1] + i[0]];
}
Vec3& Image::operator[](const std::array<int, 2>& i) {
	return vec[width * i[1] + i[0]];
}

Vec3 Image::color(float x, float y) const {
	if (halfed && y > 0.5) return Vec3(0);
	int xx = (int)((x - std::floor(x)) * width);
	int yy = (int)((y - std::floor(y)) * height);
	Vec3 v = vec[width * yy + xx];
	return v;
}
Image raytrace(size_t threadcount, const std::shared_ptr<Scene>& scene,
               size_t n) {
	Image result(scene->cam.width, scene->cam.height);

	result.setPixels(threadcount, std::make_shared<StratifiedSampler>(
	                                  StratifiedSampler(scene, n)));
	result.gammaCorrect(2.2);
	return result;
}
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

	std::ofstream ofile(filename, std::ios::binary);

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
			red = std::clamp<float>((img[{x, y}][0]), 0, 1) * 255;
			green = std::clamp<float>((img[{x, y}][1]), 0, 1) * 255;
			blue = std::clamp<float>((img[{x, y}][2]), 0, 1) * 255;

			// bmp colors are bgr not rgb
			char bgr[3] = {blue, green, red};
			for (int i = 0; i < 3; i++) {
				char c0 = (bgr[i] & 0x00FF);

				ofile.write(&c0, sizeof(c0));
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

Image readImage(const char* filename) {
	int width, height, channels;
	unsigned char* img = stbi_load(filename, &width, &height, &channels, 0);
	Image result(width, height);
	if (channels != 3)
		std::cout << "Careful! Loaded image has " << channels << " channels"
		          << std::endl;
	size_t img_size = width * height * channels;
	int i = 0;
	for (unsigned char* p = img; p != img + img_size; p += channels) {
		float x = (float)*(p + 0) / 255;
		float y = (float)*(p + 1) / 255;
		float z = (float)*(p + 2) / 255;
		result[{i, 0}] = Vec3(x, y, z);
		i++;
	}
	// Reverse the gamma correction on loaded images
	result.gammaCorrect(0.454545);
	return result;
}
}  // namespace util