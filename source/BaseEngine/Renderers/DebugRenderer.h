#pragma once
#include <vector>
#include "../Loader/Face.h"
#include <GL/glew.h>
namespace DebugMeshes
{
	enum Ids
	{
		SPHERE_PLANE = 0,
		COUNT
	};
}
class CDebugRenderer
{
public:
	void DrawLineMesh(const glm::mat4& MVP ,const std::vector<SFace>& faces) const;
	int m_ModelsId[DebugMeshes::COUNT];
};