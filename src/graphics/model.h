#include "../red_common.h"
#include <indexgenerator.h>
typedef struct
{
	float vx, vy, vz;
	float nx, ny, nz, nw;
	float tu, tv;
} Vertex;

FAST_VECTOR(Vertex);

typedef struct
{
	fast_vector_Vertex vertices;
	fast_vector_u32 indices;
} Mesh;
#include <fast_obj.h>

static inline Mesh loadMesh_fast(const char* path)
{
	fastObjMesh* obj = fast_obj_read(path);

	size_t totalIndices = 0;

	for (u32 i = 0; i < obj->face_count; i++)
	{
		totalIndices += 3 * (obj->face_vertices[i] - 2);
	}

	//vector<Vertex> vertices(totalIndices);
	fast_vector_Vertex vertices;
	fast_vector_allocate(Vertex, vertices, totalIndices);

	size_t vertexOffset = 0;
	size_t indexOffset = 0;

	for (u32 i = 0; i < obj->face_count; i++)
	{
		for (u32 j = 0; j < obj->face_vertices[i]; j++)
		{
			fastObjIndex gi = obj->indices[indexOffset + j];
						
			Vertex v;
			v.vx = obj->positions[gi.p * 3 + 0];
			v.vy = obj->positions[gi.p * 3 + 1];
			v.vz = obj->positions[gi.p * 3 + 2];

			v.nx = obj->positions[gi.p * 3 + 0];
			v.ny = obj->positions[gi.p * 3 + 1];
			v.nz = obj->positions[gi.p * 3 + 2];
			
			v.tu = obj->texcoords[gi.t * 2 + 0];
			v.tv = 1.0f - obj->texcoords[gi.t * 2 + 1]; // originally: obj->texcoords[gi.t * 2 + 1]

			if (j >= 3)
			{
				vertices.data[vertexOffset + 0] = vertices.data[vertexOffset - 3];
				vertices.data[vertexOffset + 1] = vertices.data[vertexOffset - 1];
				vertexOffset += 2;
			}

			vertices.data[vertexOffset] = v;
			vertexOffset++;
		}

		indexOffset += obj->face_vertices[i];
	}

	fast_obj_destroy(obj);

	Mesh result;

	fast_vector_u32 remap;
	fast_vector_allocate(u32, remap, totalIndices);

	size_t totalVertices = meshopt_generateVertexRemap(&remap.data[0], null, totalIndices, &vertices.data[0], totalIndices, sizeof(Vertex));
	
	fast_vector_allocate(u32, result.indices, totalIndices);
	fast_vector_allocate(Vertex, result.vertices, totalVertices);

	meshopt_remapIndexBuffer(&result.indices.data[0], nullptr, totalIndices, &remap.data[0]);
	meshopt_remapVertexBuffer(&result.vertices.data[0], &vertices.data[0], totalIndices, sizeof(Vertex), &remap.data[0]);

	return result;
}