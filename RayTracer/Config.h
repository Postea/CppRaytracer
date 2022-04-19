#include <map>

#include "tools/Mat4.h"
using namespace util;
namespace config {

std::map<std::string, util::Mat4> cameras{
    {"upperAngle", rotate(Vec3(1, 0, 0), -40) * rotate(Vec3(0, 1, 0), -45) *
                       translate(Vec3(-4, 4, 5))},
    {"lowerAngle", rotate(Vec3(1, 0, 0), 15) * translate(Vec3(0.5, 0.2, 6))},
    {"topDown", rotate(Vec3(1, 0, 0), -90) * rotate(Vec3(1, 0, 0), 0) *
                    translate(Vec3(0, 7, 0))},
};
std::map<std::string, std::string> skies{
    {"FullMoon", "textures/FullMoon.bmp"},
};

std::string file_name(size_t n, size_t l, size_t d, std::string camera_key,
                      std::string sky_key) {
	// derived
	size_t n_n = n * n;

	std::string fnme("results/outdoor/");
	fnme.append(camera_key);
	fnme.append("_");
	fnme.append(std::to_string(n_n));
	fnme.append("_");
	fnme.append(std::to_string(l));
	fnme.append("_");
	fnme.append(std::to_string(d));
	fnme.append("_");
	fnme.append(sky_key);
	fnme.append(".bmp");
	return fnme;
};
}  // namespace config