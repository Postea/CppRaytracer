#include <map>

#include "tools/Mat4.h"
using namespace util;
namespace config {

std::map<std::string, util::Mat4> cameras{
    {"topDown", rotate(Vec3(1, 0, 0), -90) * translate(Vec3(0, 9.5, -1))}};

std::string file_name(size_t n, size_t l, size_t d, std::string camera_key) {
	// derived
	size_t n_n = n * n;

	std::string fnme("results/depthvariance/");
	fnme.append(camera_key);
	fnme.append("_");
	fnme.append(std::to_string(n_n));
	fnme.append("_");
	fnme.append(std::to_string(l));
	fnme.append("_");
	fnme.append(std::to_string(d));
	fnme.append("_");
	fnme.append(".bmp");
	return fnme;
};
}  // namespace config