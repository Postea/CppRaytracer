#pragma once

#include "ObjectLoader.h"

#include <sstream>

#include "Vertex.h"

namespace util {
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
std::vector<shapes::Triangle> load_obj(
    std::istream& in, const std::shared_ptr<material::Material>& material) {
	std::vector<Vertex> vertices;
	std::vector<Vec3> v;
	// std::vector<Vec3> vt;
	// std::vector<Vec3> vn;

	std::string line_str;
	while (std::getline(in, line_str)) {
		std::istringstream line_ss(line_str);
		std::string line_type;
		line_ss >> line_type;

		// vertex
		if (line_type == "v") {
			float x = 0, y = 0, z = 0;
			line_ss >> x >> y >> z;
			v.push_back(Vec3(x, y, z));
		}

		// texture
		/*if (lineType == "vt") {
		    float u = 0, v = 0;
		    lineSS >> u >> v;
		    vt.push_back(Vec3(u, v, 0));
		}*/

		// normal
		/*if (lineType == "vn") {
		    float i = 0, j = 0, k = 0;
		    lineSS >> i >> j >> k;
		    vn.push_back(Vec3(i, j, k).normalize());
		}*/

		// polygon
		if (line_type == "f") {
			std::string ref_str;
			while (line_ss >> ref_str) {
				std::istringstream ref(ref_str);
				std::string v_str
				    //, vtStr, vnStr
				    ;
				std::getline(ref, v_str, '/');
				// std::getline(ref, vtStr, '/');
				// std::getline(ref, vnStr, '/');
				int currentv = atoi(v_str.c_str());
				// int currentvt = atoi(vtStr.c_str());
				// int currentvn = atoi(vnStr.c_str());
				currentv = (currentv >= 0 ? currentv : v.size() + currentv);
				/*currentvt =
				    (currentvt >= 0 ? currentvt : vt.size() + currentvt);
				currentvn =
				    (currentvn >= 0 ? currentvn : vn.size() + currentvn);*/
				Vertex vert;
				vert.position = v[currentv - 1];
				/*vert.texcoord = v[currentvt - 1];
				vert.normal = v[currentvn - 1];*/
				vertices.push_back(vert);
			}
		}
	}
	std::vector<shapes::Triangle> triangles;
	for (int i = 0; i < vertices.size(); i += 3) {
		triangles.push_back(
		    {vertices[i + 0], vertices[i + 1], vertices[i + 2], material});
	}
	return triangles;
}
}  // namespace util