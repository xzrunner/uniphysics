#include "uniphysics/cloth/ClothMeshData.h"

#include <NvClothExt/ClothMeshDesc.h>

namespace
{

template <typename T>
static nv::cloth::BoundedData ToBoundedData(T& vector)
{
	nv::cloth::BoundedData d;
	d.data   = &vector[0];
	d.stride = sizeof(vector[0]);
	d.count  = (physx::PxU32)vector.size();
	return d;
}

}

namespace up
{
namespace cloth
{

void ClothMeshData::GeneratePlaneCloth(float width, float height, int segments_x, int segments_y, bool create_quads,
                                       const sm::mat4& transform, bool alternating_diagonals, int zigzag)
{
/*
GeneratePlaneCloth(x,y,2,2) generates:

    v0______v1_____v2     v0______v1_____v2
     |      |      |       |\     |\     |
     |  Q0  |  Q1  |       |  \t0 |  \t2 |
     |      |      |       | t1 \ | t3 \ |
    v3------v4-----v5     v3-----\v4----\v5
     |      |      |       | \    | \    |
     |  Q2  |  Q3  |       |   \t4|   \t6|
     |______|______|       |_t5_\_|_t7__\|
    v6      v7     v8     v6      v7     v8
*/

	Clear();
	m_vertices.resize((segments_x + 1) * (segments_y + 1));
	m_inv_masses.resize((segments_x + 1) * (segments_y + 1));
	m_triangles.resize(segments_x * segments_y * 2);
    if (create_quads) {
        m_quads.resize(segments_x * segments_y);
    }

	m_mesh.vertices.resize(m_vertices.size());
	m_mesh.indices.resize(3 * m_triangles.size());

	sm::vec3 topLeft(-width * 0.5f, 0.f, -height * 0.5f);
//	vec3 topLeftGLM(-width * 0.5f, translateUp, -height * 0.5f);

	//calculate uv scale and offset to keep texture aspect ratio 1:1
	float uv_sx = width > height ? 1.0f : width / height;
	float uv_sy = width > height ? height / width : 1.0f;
	float uv_ox = 0.5f * (1.0f - uv_sx);
	float uv_oy = 0.5f * (1.0f - uv_sy);

	// Vertices
	for (int y = 0; y < segments_y + 1; y++)
	{
		for(int x = 0; x < segments_x + 1; x++)
		{
			sm::vec3 pos;
			switch(zigzag)
			{
				case 1:
					pos =	sm::vec3(((float)x / (float)segments_x) * width,
							sinf(y*0.5f)/(float)segments_y * height,
							((float)y / (float)segments_y) * height);
					break;
				case 2:
					pos =	sm::vec3(((float)x / (float)segments_x) * width,
							((float)(y&2) / (float)segments_y) * height,
							((float)((y+1)&~1) / (float)segments_y) * height);
					break;
				default:
					pos =	sm::vec3(((float)x / (float)segments_x) * width,
							0.f,
							((float)y / (float)segments_y) * height);
			}

			m_vertices[x + y * (segments_x + 1)] = transform * (topLeft + pos);

			m_inv_masses[x + y * (segments_x + 1)] = 1.0f;

			m_mesh.vertices[x + y * (segments_x + 1)].position = transform * (topLeft + pos);

			m_mesh.vertices[x + y * (segments_x + 1)].normal = transform * sm::vec3(0.f, 1.f, 0.f);

			m_mesh.vertices[x + y * (segments_x + 1)].uv = sm::vec2(uv_ox + uv_sx*(float)x / (float)segments_x, uv_oy + uv_sy*(1.0f - (float)y / (float)segments_y));
		}
	}

	if (create_quads)
	{
		// Quads
		for (int y = 0; y < segments_y; y++)
		{
			for (int x = 0; x < segments_x; x++)
			{
				m_quads[(x + y * segments_x)] = Quad((uint32_t)(x + 0) + (y + 0) * (segments_x + 1),
					(uint32_t)(x + 1) + (y + 0) * (segments_x + 1),
					(uint32_t)(x + 1) + (y + 1) * (segments_x + 1),
					(uint32_t)(x + 0) + (y + 1) * (segments_x + 1));
			}
		}
	}

	// Triangles
	for (int y = 0; y < segments_y; y++)
	{
		for(int x = 0; x < segments_x; x++)
		{
			if(alternating_diagonals && (x^y)&1)
			{
				//Top right to bottom left
				m_triangles[(x + y * segments_x) * 2 + 0] = Triangle( (uint32_t)(x + 0) + (y + 0) * (segments_x + 1),
																	(uint32_t)(x + 1) + (y + 0) * (segments_x + 1),
																	(uint32_t)(x + 0) + (y + 1) * (segments_x + 1));

				m_triangles[(x + y * segments_x) * 2 + 1] = Triangle( (uint32_t)(x + 1) + (y + 0) * (segments_x + 1),
																	(uint32_t)(x + 1) + (y + 1) * (segments_x + 1),
																	(uint32_t)(x + 0) + (y + 1) * (segments_x + 1));
			}
			else
			{
				//Top left to bottom right
				m_triangles[(x + y * segments_x) * 2 + 0] = Triangle( (uint32_t)(x + 0) + (y + 0) * (segments_x + 1),
																	(uint32_t)(x + 1) + (y + 0) * (segments_x + 1),
																	(uint32_t)(x + 1) + (y + 1) * (segments_x + 1));

				m_triangles[(x + y * segments_x) * 2 + 1] = Triangle( (uint32_t)(x + 0) + (y + 0) * (segments_x + 1),
																	(uint32_t)(x + 1) + (y + 1) * (segments_x + 1),
																	(uint32_t)(x + 0) + (y + 1) * (segments_x + 1));
			}
		}
	}

	for (int i = 0; i < (int)m_triangles.size(); i++)
	{
		m_mesh.indices[3 * i] = m_triangles[i].a;
		m_mesh.indices[3 * i + 1] = m_triangles[i].b;
		m_mesh.indices[3 * i + 2] = m_triangles[i].c;
	}
}

void ClothMeshData::AttachClothPlaneByAngles(int segments_x, int segments_y, bool attach_by_width)
{
    for (int y = 0; y < segments_y + 1; y++) {
        for (int x = 0; x < segments_x + 1; x++) {
            if ((attach_by_width && y == 0) || (!attach_by_width && x == 0)) {
                if (x == 0 || x == segments_x) {
                    m_inv_masses[x + y * (segments_x + 1)] = 0.0f;
                }
            }
        }
    }
}

void ClothMeshData::SetInvMasses(float inv_mass)
{
    // Doesn't modify attached vertices
    for (int i = 0; i < (int)m_inv_masses.size(); ++i) {
        if (m_inv_masses[i] > 1e-6f) {
            m_inv_masses[i] = inv_mass;
        }
    }
}

std::unique_ptr<::nv::cloth::ClothMeshDesc> ClothMeshData::GetClothMeshDesc() const
{
    auto d = std::make_unique<::nv::cloth::ClothMeshDesc>();

    d->setToDefault();
    d->points = ToBoundedData(m_vertices);
    if (m_quads.size() != 0) {
        d->quads = ToBoundedData(m_quads);
    }
    if (m_triangles.size() != 0) {
        d->triangles = ToBoundedData(m_triangles);
    }
    d->invMasses = ToBoundedData(m_inv_masses);

    return d;
}

void ClothMeshData::Merge(const ClothMeshData& other)
{
	uint32_t firstVertex = (uint32_t)m_vertices.size();
	uint32_t firstTriangle = (uint32_t)m_triangles.size();
	uint32_t firstQuad = (uint32_t)m_quads.size();

	m_vertices.insert(m_vertices.end(), other.m_vertices.begin(), other.m_vertices.end());
//	mUvs.insert(mUvs.end(), other.mUvs.begin(), other.mUvs.end());
	m_inv_masses.insert(m_inv_masses.end(), other.m_inv_masses.begin(), other.m_inv_masses.end());

	m_mesh.vertices.insert(m_mesh.vertices.end(), other.m_mesh.vertices.begin(), other.m_mesh.vertices.end());

	for(const auto& t : other.m_triangles) {
		m_triangles.push_back(t + firstVertex);
	}
	for(const auto& q : other.m_quads)
	{
		m_quads.push_back(q + firstVertex);
		m_mesh.indices.push_back(m_quads.back().a);
		m_mesh.indices.push_back(m_quads.back().b);
		m_mesh.indices.push_back(m_quads.back().c);
	}
}

void ClothMeshData::Clear()
{
    m_vertices.clear();
    m_triangles.clear();
    m_quads.clear();
}

}
}