#include "EnitityRenderer.h"

void CEntityRenderer::Render(CScene* scene, const CEntityGeometryPassShader& geomentry_shader)
{
	m_RendererObjectPerFrame = 0;
	m_RendererVertixesPerFrame = 0;	

	for (const auto& entity : scene->GetEntitiesInCameraRange())
	{
		CModel* model = scene->GetLoader().m_Models[entity->GetModelId()].get();
		RenderEntity(scene, entity, *model, geomentry_shader);
		m_RendererObjectPerFrame++;
	}
}
void CEntityRenderer::RenderModel(CScene* scene, CModel & model, const CEntityGeometryPassShader & geomentry_shader,
	const int& transform_matrixes_size,
	const std::vector<glm::mat4>& transform_matrixes,
	const glm::mat4& relative_matrix,
	const int& current_frame
)
{
	geomentry_shader.LoadUseFakeLight(static_cast<float>(model.m_UseFakeLight));

	unsigned int mesh_id = 0;
	for (const auto& mesh : model.GetModifyMeshes())
	{
		if (!mesh.IsReadyToRender())
			continue;

		glBindVertexArray(mesh.GetVao());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		if (model.UseInstacedRendering())
			glEnableVertexAttribArray(4);
		if (mesh.UseBoneTransform())
		{
			glEnableVertexAttribArray(8);
			glEnableVertexAttribArray(9);
		}

		int is_texture = 0, is_normal_map = 0, is_specular_map = 0, is_blend_map = 0;
		int i = 0;
		for (const auto& td : mesh.GetMaterial().textures)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			CTextInfo* t = scene->GetLoader().m_Textures[td].get();
			glBindTexture(GL_TEXTURE_2D, t->GetId());

			switch (t->type)
			{
			case MaterialTexture::DIFFUSE:	 is_texture++;		break;
			case MaterialTexture::NORMAL:	 is_normal_map++;   break;
			case MaterialTexture::SPECULAR:  is_specular_map++; break;
			case MaterialTexture::BLEND_MAP: is_blend_map++;	break;
			}
			i++;
		}
		if (is_normal_map > 0)
			geomentry_shader.LoadUseNormalMap(1.0f);
		else
			geomentry_shader.LoadUseNormalMap(0.0f);

		geomentry_shader.LoadMeshMaterial(mesh.GetMaterial());

		GLenum render_mode =  GL_TRIANGLES; //GL_LINES

		if (mesh.GetMaterial().isTransparency)
			Utils::DisableCulling();			

		geomentry_shader.LoadUseBonesTransformation(static_cast<float>(mesh.UseBoneTransform()));
		if (model.m_BoneUpdate)
		{
			const std::vector<glm::mat4>& transforms = model.GetBonesTransforms(current_frame);// m_BoneTransformMatrixes;
	
			int mid = 0;
			for (const glm::mat4& m : transforms)
			{
				geomentry_shader.LoadBoneTransform(m, mid++);
			}
		}

		if (model.UseInstacedRendering())
		{
			geomentry_shader.LoadUseInstancedRendering(1.f);
			glDrawElementsInstanced(render_mode, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0, transform_matrixes_size);
		}
		else
		{
			for (const glm::mat4& mat : transform_matrixes)
			{
				geomentry_shader.LoadUseInstancedRendering(0.f);
				geomentry_shader.LoadTransformMatrix(relative_matrix * mat);
				glDrawElements(render_mode, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0);
			}
		}
		if (m_MaxVerices < mesh.GetVertexCount())
			m_MaxVerices = mesh.GetVertexCount();
		m_RendererVertixesPerFrame += mesh.GetVertexCount() * transform_matrixes.size();

		glDisable(GL_TEXTURE_2D);

		if (mesh.GetMaterial().isTransparency)
			Utils::EnableCulling();

		if (mesh.UseBoneTransform())
		{
			glDisableVertexAttribArray(9);
			glDisableVertexAttribArray(8);
		}
		if (model.UseInstacedRendering())
			glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}
const unsigned int & CEntityRenderer::GetObjectsPerFrame()
{
	return m_RendererObjectPerFrame;
}
const unsigned int & CEntityRenderer::GetVertexPerFrame()
{
	return m_RendererVertixesPerFrame;
}
void CEntityRenderer::LoadUniforms(CModel & model, const CEntityGeometryPassShader & geomentry_shader,
	const int& transform_matrixes_size,
	const std::vector<glm::mat4>& transform_matrixes,
	const glm::mat4& relative_matrix)
{
	geomentry_shader.LoadUseFakeLight(static_cast<float>(model.m_UseFakeLight));

	unsigned int mesh_id = 0;
	for (const auto& mesh : model.GetModifyMeshes())
	{
		geomentry_shader.LoadUseBonesTransformation(static_cast<float>(mesh.UseBoneTransform()));
	}
}
void CEntityRenderer::RenderEntity(CScene* scene, CEntity* entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader)
{
	RenderModel(scene, model, geomentry_shader, entity->GetTransformMatrixes().size() ,entity->GetTransformMatrixes(), entity->GetRelativeTransformMatrix(), entity->GetAnimationFrame());
}