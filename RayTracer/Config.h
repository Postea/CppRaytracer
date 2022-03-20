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
    {"StaryNight", "textures/moonlit_golf_4k.hdr"},
    {"FullMoon", "textures/FullMoon4.bmp"},
};

size_t threadpool_size = 4;

size_t sample_n = 1;
size_t sample_l = 9;
size_t max_depth = 8;

auto camera_key = "topDown";
auto sky_key = "StaryNight";

// derived
size_t n_n = sample_n * sample_n;
std::string formula = std::to_string(n_n) + "*(1+" + std::to_string(sample_l) +
                      ") = " + std::to_string((n_n) * (1 + sample_l));
bool direct = sample_l != 0;
auto camera = cameras[camera_key];
auto sky = skies[sky_key];
std::string file_name() {
	std::string fnme("results/");
	if (direct)
		fnme.append("direct_");
	else
		fnme.append("classi_");
	fnme.append(sky_key);
	fnme.append("_");
	fnme.append(camera_key);
	fnme.append("_");
	fnme.append(std::to_string(n_n));
	fnme.append("_");
	if (direct)
		fnme.append(std::to_string(sample_l));
	else
		fnme.append(std::to_string(0));
	fnme.append(".bmp");
	return fnme;
};
auto fnme = file_name();
}  // namespace config