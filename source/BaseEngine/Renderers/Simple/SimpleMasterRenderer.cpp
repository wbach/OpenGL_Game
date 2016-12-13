#include "SimpleMasterRenderer.h"

void CSimpleMasterRenderer::Init(CCamera * camera, glm::vec2 window_size, glm::mat4 & projection_matrix, float view_distance)
{
	m_ProjectionMatrix = projection_matrix;
	m_WindowSize = window_size;
	m_ViewDistance = view_distance + 60;

	m_SimpleEntityShader.Init();
	m_SimpleEntityShader.Start();
	m_SimpleEntityShader.LoadProjectionMatrix(projection_matrix);
	//m_SimpleEntityShader.LoadViewDistance(view_distance + 50);
	m_SimpleEntityShader.Stop();

	m_SimpleTerrainShader.Init();
	m_SimpleTerrainShader.Start();
	m_SimpleTerrainShader.LoadProjectionMatrix(projection_matrix);
	//m_SimpleTerrainShader.LoadViewDistance(view_distance + 50);
	m_SimpleTerrainShader.Stop();

	m_SkyBoxRenderer.Init(projection_matrix, 1000, "Data/Shaders/Simple/SkyboxSimpleShader.vert", "Data/Shaders/Simple/SkyboxSimpleShader.frag");
}

void CSimpleMasterRenderer::Render(CScene* scene)
{
	std::list<CTerrain*> current_terrains = scene->GetTerrainsInCameraRange();

	scene->CheckObjectsInCameraRange();

	m_SkyBoxRenderer.Render(scene->GetViewMatrix(), scene->m_DayNightCycle.GetDeltaTime(), scene->m_DayNightCycle.GetDayNightBlendFactor());

	m_SimpleEntityShader.Start();
	m_SimpleEntityShader.LoadViewMatrix(scene->GetViewMatrix());
	//m_SimpleEntityShader.LoadProjectionMatrix(m_ProjectionMatrix);
	for (auto entity : scene->GetEntitiesInCameraRange())
	{
		CModel* model = scene->GetLoader().m_Models[entity->GetModelId()].get();
		RenderEntity(scene, entity, *model);
		// **************************************Trees use entity render******************************************
		for (const auto& terrain : current_terrains)
		{
			for (auto& model : terrain->m_Trees)
			{
				CModel* m = scene->GetLoader().m_Models[model].get();
				RenderEntity(scene, nullptr, *m, m->GetInstancedSize());
			}
		}
	}
	m_SimpleEntityShader.Stop();



	m_SimpleTerrainShader.Start();
	m_SimpleTerrainShader.LoadViewMatrix(scene->GetViewMatrix());
	for (const auto& terrain : current_terrains)
	{
		if (!terrain->m_Model.GetMeshes()[0].IsReadyToRender() || !terrain->m_IsInit)
			continue;

		glBindVertexArray(terrain->m_Model.GetMeshes()[0].GetVao());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain->m_BackgroundTexture[0]]->GetId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain->m_RTexture[0]]->GetId());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain->m_GTexture[0]]->GetId());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain->m_BTexture[0]]->GetId());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain->m_BlendMap]->GetId());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain->m_RockTexture[0]]->GetId());

		m_SimpleTerrainShader.LoadTransformMatrix(Utils::CreateTransformationMatrix(glm::vec3(terrain->m_Transform.position.x, 0, terrain->m_Transform.position.z), glm::vec3(0), glm::vec3(1)));
		glDrawElements(GL_TRIANGLE_STRIP, terrain->m_Model.GetMeshes()[0].GetVertexCount(), GL_UNSIGNED_SHORT, 0);
		
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	m_SimpleTerrainShader.Stop();
}
void CSimpleMasterRenderer::SetSkyBoxTextures(CTextInfo* day, CTextInfo*  night)
{
	m_SkyBoxRenderer.SetTextures(day, night);
}
void CSimpleMasterRenderer::SetSkyBoxMesh(const CMesh* mesh)
{
	m_SkyBoxRenderer.SetMesh(mesh);
}
void CSimpleMasterRenderer::RenderEntity(CScene* scene, CEntity* entity, CModel & model, const int& instaced_size) const
{

	for (const CMesh& mesh : model.GetMeshes())
	{
		if (!mesh.IsReadyToRender())
			continue;

		glBindVertexArray(mesh.GetVao());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		if (model.UseInstacedRendering())
			glEnableVertexAttribArray(4);
		if (mesh.UseBoneTransform())
		{
			glEnableVertexAttribArray(8);
			glEnableVertexAttribArray(9);
		}

		int i = 0;
		for (const auto& td : mesh.GetMaterial().textures)
		{
			if (scene->GetLoader().m_Textures[td]->type != MaterialTexture::DIFFUSE)
				continue;
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[td]->GetId());
		}

		m_SimpleEntityShader.LoadUseBonesTransformation(static_cast<float>(mesh.UseBoneTransform()));
		if (entity != nullptr)
		{
			if (model.m_BoneUpdate)
			{
				const std::vector<glm::mat4>& transforms = model.GetBonesTransforms(entity->GetAnimationFrame());// m_BoneTransformMatrixes;

				int mid = 0;
				for (const glm::mat4& m : transforms)
				{
					m_SimpleEntityShader.LoadBoneTransform(m, mid++);
				}
			}
		}
		

		if (model.UseInstacedRendering())
		{
			m_SimpleEntityShader.LoadUseInstancedRendering(1.f);
			int size = entity != nullptr ? entity->GetTransformMatrixes().size() : instaced_size;
			glDrawElementsInstanced(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0, size);
		}
		else
		{
			for (const glm::mat4& mat : entity->GetTransformMatrixes())
			{
				m_SimpleEntityShader.LoadUseInstancedRendering(0.f);
				m_SimpleEntityShader.LoadTransformMatrix(entity->GetRelativeTransformMatrix() * mat);
				glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0);
			}
		}
		if (mesh.UseBoneTransform())
		{
			glDisableVertexAttribArray(9);
			glDisableVertexAttribArray(8);
		}
		if (model.UseInstacedRendering())
			glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}