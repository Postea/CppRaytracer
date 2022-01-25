#include <map>

#include "tools/Mat4.h"
using namespace util;
namespace config {

std::map<std::string, util::Mat4> cameras{
    {"upperAngle", rotate(Vec3(1, 0, 0), -30) * rotate(Vec3(0, 1, 0), -45) *
                       translate(Vec3(0, 3, 2))},
    {"topDown", rotate(Vec3(1, 0, 0), -90) * rotate(Vec3(1, 0, 0), 0) *
                    translate(Vec3(0, 3, 0))},
};
std::map<std::string, std::string> skies{
    {"StaryNight", "textures/moonlit_golf_4k.hdr"},
    {"FullMoon", "textures/FullMoon4.bmp"},
};

size_t threadpool_size = 4;

size_t sample_n = 100;
size_t sample_l = 0;
size_t max_depth = 8;

auto camera_key = "upperAngle";
auto sky_key = "FullMoon";

// derived
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
	fnme.append(std::to_string(sample_n));
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