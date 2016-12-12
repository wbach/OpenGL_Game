#include "ShadowMapRenderer.h"

CShadowMapRenderer::CShadowMapRenderer()
{
	m_ShadowMapSize = 4096;
	m_Offset = Utils::CreateOffset();	
}

void CShadowMapRenderer::Init(CCamera* camera, glm::vec2 window_size, float fov, float near_plane, float shadow_map_size, float shadows_distance)
{
	m_IsInitialized = true;
	m_ShadowMapSize = shadow_map_size;
	m_ShadowShader.Init();
	m_ShadowFbo.Init(glm::vec2(m_ShadowMapSize), window_size);
	m_ShadowBox.Init(camera, window_size, fov, near_plane, shadows_distance);
	m_LightViewMatrix = glm::mat4(1.f);
	m_ShadowBox.SetLightViewMatrix(m_LightViewMatrix);
}
void CShadowMapRenderer::Render(CScene* scene)
{
	if (!m_IsInitialized)
	{
		return;
	}	

	glm::vec3 light_direction;
	try
	{
		light_direction = scene->GetDirectionalLightPosition() * -1.f;
	}
	catch (const std::exception& e)
	{
		e.what();
		return;
	}
	//if (scene->GetEntities().size() <= 0) return;
	m_ShadowFbo.BindFBO();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_ShadowBox.Update();

	Prepare(light_direction, m_ShadowBox);

	m_ShadowShader.Start();
	m_ShadowShader.LoadProjectionMatrix(m_ProjectionViewMatrix);
	m_ShadowShader.LoadViewMatrix(scene->GetViewMatrix());

	for (CEntity* entity : scene->GetEntitiesInCameraRange())
	{
		CModel* model = scene->GetLoader().m_Models[entity->GetModelId()].get();
		RenderEntity(scene, entity, *model);
	}
	m_ShadowShader.Stop();
	Finish();
}

void CShadowMapRenderer::SetShadowMapSize(float size)
{
	m_ShadowMapSize = size;
}

const GLuint& CShadowMapRenderer::GetShadowMap() const
{
	return m_ShadowFbo.GetShadowMap();
}

const glm::mat4 CShadowMapRenderer::GetToShadowMapSpaceMatrix() const
{
	return m_Offset * m_ProjectionViewMatrix;
}

void CShadowMapRenderer::RenderEntity(CScene* scene, CEntity* entity, CModel & model) const
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
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[td]->GetId());			
		}

		m_ShadowShader.LoadUseBonesTransformation(static_cast<float>(mesh.UseBoneTransform()));
		if (model.m_BoneUpdate)
		{
			const std::vector<glm::mat4>& transforms = model.GetBonesTransforms(entity->GetAnimationFrame());// m_BoneTransformMatrixes;

			int mid = 0;
			for (const glm::mat4& m : transforms)
			{
				m_ShadowShader.LoadBoneTransform(m, mid++);
			}
		}

		if (model.UseInstacedRendering())
		{
			m_ShadowShader.LoadUseInstancedRendering(1.f);
			glDrawElementsInstanced(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0, entity->GetTransformMatrixes().size());
		}
		else
		{
			for (const glm::mat4& mat : entity->GetTransformMatrixes())
			{
				m_ShadowShader.LoadUseInstancedRendering(0.f);
				m_ShadowShader.LoadTransformMatrix(entity->GetRelativeTransformMatrix() * mat);
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

void CShadowMapRenderer::Prepare(const glm::vec3 & light_direction, const CShadowBox & box)
{
	UpdateOrthoProjectionMatrix(box.GetWidth(), box.GetHeight(), box.GetLength());
	UpdateLightViewMatrix(light_direction, box.GetCenter());
	m_ShadowBox.SetLightViewMatrix(m_LightViewMatrix);
	m_ProjectionViewMatrix = m_ProjectonMatrix * m_LightViewMatrix;
}

void CShadowMapRenderer::Finish()
{
	m_ShadowFbo.UnbindFrameBuffer();
}

void CShadowMapRenderer::UpdateLightViewMatrix(glm::vec3 direction, glm::vec3 center)
{
	direction = glm::normalize(direction);
	center *= -1.0f;
	m_LightViewMatrix = glm::mat4(1.0);

	float length = glm::length(glm::vec2(direction.x, direction.z));
	float pitch = (float)Utils::ToDegrees(acos(length));
	m_LightViewMatrix *= glm::rotate(pitch, glm::vec3(1, 0, 0));
	float yaw = (float)Utils::ToDegrees(((float)atan(direction.x / direction.z)));
	yaw = direction.z > 0 ? yaw - 180 : yaw;
	m_LightViewMatrix *= glm::rotate((float)-(yaw), glm::vec3(0, 1, 0));
	m_LightViewMatrix *= glm::translate(center);	
}

void CShadowMapRenderer::UpdateOrthoProjectionMatrix(float width, float height, float length)
{
	m_ProjectonMatrix = glm::mat4(1.f);
	m_ProjectonMatrix[0][0] = 2.f / width;
	m_ProjectonMatrix[1][1] = 2.f / height;
	m_ProjectonMatrix[2][2] = -2.f / length;
	m_ProjectonMatrix[3][3] = 1.f;
}