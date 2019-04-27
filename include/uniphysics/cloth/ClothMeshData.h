#pragma once

#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <vector>
#include <memory>

namespace nv { namespace cloth { class ClothMeshDesc; } }

namespace up
{
namespace cloth
{

struct SimpleMesh
{
	class Vertex
	{
	public:
		sm::vec3 position;
		sm::vec3 normal;
		sm::vec2 uv;
	};

//	virtual uint32_t getVertexStride() const { return sizeof(Vertex); }

	std::vector<Vertex>   vertices;
	std::vector<uint16_t> indices;

	sm::vec3 extents;
	sm::vec3 center;

}; // SimpleMesh

class ClothMeshData
{
public:
    void GeneratePlaneCloth(float width, float height, int segments_x, int segments_y, bool create_quads = false,
        const sm::mat4& transform = sm::mat4(), bool alternating_diagonals = true, int zigzag = 0);

    void AttachClothPlaneByAngles(int segments_x, int segments_y, bool attach_by_width = true);

    void SetInvMasses(float inv_mass);

    std::unique_ptr<::nv::cloth::ClothMeshDesc> GetClothMeshDesc() const;
    auto& GetRenderMesh() const { return m_mesh; }

    void Merge(const ClothMeshData& other);

    auto& Vertices() { return m_vertices; }

    float GetInvMass(int index) {
        return index >= 0 && index < static_cast<int>(m_inv_masses.size()) ? m_inv_masses[index] : 0;
    }

    void Clear();

private:
    struct Triangle
    {
        uint32_t a, b, c;

        Triangle() {}
        Triangle(uint32_t _a, uint32_t _b, uint32_t _c) : a(_a), b(_b), c(_c) {}

        Triangle operator + (uint32_t offset)const { return Triangle(a + offset, b + offset, c + offset); }

    }; // Triangle

    struct Quad
    {
        uint32_t a, b, c, d;

        Quad() {}
        Quad(uint32_t _a, uint32_t _b, uint32_t _c, uint32_t _d) : a(_a), b(_b), c(_c), d(_d) {}

        Quad operator + (uint32_t offset)const { return Quad(a + offset, b + offset, c + offset, d + offset); }

    }; // Quad

private:
    std::vector<sm::vec3> m_vertices;
    std::vector<Triangle> m_triangles;
    std::vector<Quad>     m_quads;
    std::vector<float>    m_inv_masses;

    SimpleMesh m_mesh;

}; // ClothMeshData

}
}