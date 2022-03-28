#include <map>

#include "tools/Mat4.h"
using namespace util;
namespace config {

std::map<std::string, util::Mat4> cameras{
    {"upperAngle", rotate(Vec3(1, 0, 0), -10) * rotate(Vec3(0, 1, 0), 40) *
                       translate(Vec3(4, 4, 4))},
    {"topDown", rotate(Vec3(1, 0, 0), -90) * translate(Vec3(0, 9.5, -1))}};

size_t threadpool_size = 4;

size_t sample_n = 7;
size_t sample_l = 1;
size_t max_depth = 8;

auto camera_key = "upperAngle";

// derived
size_t n_n = sample_n * sample_n;
std::string formula = std::to_string(n_n) + "*(1+" + std::to_string(sample_l) +
                      ") = " + std::to_string((n_n) * (1 + sample_l));
bool direct = sample_l != 0;
auto camera = cameras[camera_key];
std::string file_name() {
	std::string fnme("results/indoor/");
	if (direct)
		fnme.append("direct_");
	else
		fnme.append("classi_");
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