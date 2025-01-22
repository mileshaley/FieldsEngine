/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: mesh_import.cpp                                                     *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "mesh_import.h"

#include <filesystem>
#include "rapidobj/include/rapidobj/rapidobj.hpp"
#include "vector_util.h"

#include <iostream>

/*~-------------------------------------------------------------------------~*\
 * Mesh Mmport Definitions                                                   *
\*~-------------------------------------------------------------------------~*/

fe::json fields_engine::import_vis_mesh(std::filesystem::path const& in_path) {
	if (in_path.extension() == ".obj") {
        rapidobj::Result result = rapidobj::ParseFile(in_path);
        if (result.error) {
            std::cerr << result.error.code.message() << '\n';
            return nullptr;
        }
        if (!rapidobj::Triangulate(result)) {
            return nullptr;
        }
		json out;
        json& out_positions = (out["positions"] = json::array());
        json& out_normals   = (out["normals"] = json::array());
        json& out_tex_uvs   = (out["tex_uvs"] = json::array());
        json& out_tangents  = (out["tangents"] = json::array()); /// Empty for now
        json& out_sections  = (out["sections"] = json::array());
        json& out_triangles = (out["triangles"] = json::array());

        auto& positions = result.attributes.positions;
        auto& normals = result.attributes.normals;
        auto& tex_uvs = result.attributes.texcoords;
        auto& shapes = result.shapes;
        for (auto const& shape : shapes) {
            auto const& indices = shape.mesh.indices;
            //if (indices.empty()) { continue; }
            const int first_idx = int(out_triangles.size());
            for (size_t i = 0; i < indices.size(); ++i) {
                rapidobj::Index const& index = indices[i];
                out_triangles.emplace_back(out_triangles.size());
                const vec3 vert = vec_y_up_to_z_up(reinterpret_cast<vec3&>(positions[index.position_index * 3ll]));
                out_positions.emplace_back(vert.x);
                out_positions.emplace_back(vert.y);
                out_positions.emplace_back(vert.z);
                if (index.normal_index == -1) {
                    out_normals.emplace_back(0.0f);
                    out_normals.emplace_back(0.0f);
                    out_normals.emplace_back(0.0f);
                    // Default to normals facing directly out of vertices
                    //out_normals.emplace_back(vert.x);
                    //out_normals.emplace_back(vert.y);
                    //out_normals.emplace_back(vert.z);
                } else {
                    const vec3 norm = vec_y_up_to_z_up(reinterpret_cast<vec3&>(normals[index.normal_index * 3ll]));
                    out_normals.emplace_back(norm.x);
                    out_normals.emplace_back(norm.y);
                    out_normals.emplace_back(norm.z);
                }
                if (index.texcoord_index == -1) {
                    out_tex_uvs.emplace_back(0.0f);
                    out_tex_uvs.emplace_back(0.0f);
                    // Default to normals mapped directly to xy of vertices
                    //out_tex_uvs.emplace_back(vert.x);
                    //out_tex_uvs.emplace_back(vert.y);
                } else {
                    const vec2 tex_uv = reinterpret_cast<vec2&>(tex_uvs[index.texcoord_index * 3ll]);
                    out_tex_uvs.emplace_back(tex_uv.x);
                    out_tex_uvs.emplace_back(tex_uv.y);
                }
            }
            // We assume all material ids are the same and the mesh has been sectioned by material
            
            json section{};
            section["first_index"] = first_idx;
            section["index_count"] = static_cast<int>(out_triangles.size() - first_idx);
            if (shape.mesh.material_ids.empty()) {
                section["material_index"] = 0;
            } else {
                section["material_index"] = shape.mesh.material_ids[0];
            }
            out_sections.push_back(section);
        }

        return out;
	} else {
		
	}
	return nullptr;
}
