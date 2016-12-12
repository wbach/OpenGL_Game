#include "DebugRenderer.h"

void CDebugRenderer::DrawLineMesh(const glm::mat4& MVP, const std::vector<SFace>& faces) const
{
	glUseProgram(0);
	float matrix[16];
	int a = 0;
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			matrix[a++] = MVP[x][y];
	glLoadMatrixf(matrix);
	for (const SFace& face : faces)
	{
		glBegin(GL_LINES);
		glVertex3f(face.vertexes.v1.x, face.vertexes.v1.y, face.vertexes.v1.z);
		glVertex3f(face.vertexes.v2.x, face.vertexes.v2.y, face.vertexes.v2.z);
		glVertex3f(face.vertexes.v3.x, face.vertexes.v3.y, face.vertexes.v3.z);
		glEnd();
	}

}
