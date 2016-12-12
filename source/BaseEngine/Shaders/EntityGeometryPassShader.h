#pragma once
#include "ShaderProgram.h"
#include "../Loader/Material.h"

class CEntityGeometryPassShader : public CShaderProgram
{
public:
	void GetAllUniformLocations() override;
	void ConnectTextureUnits();
	void BindAttributes() override;

	void Init();
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;
	
	void LoadViewDistance(const float& distance) const;
	void LoadUseFakeLight(const float& use) const;

	void LoadUseInstancedRendering(const float& use) const;

	void LoadUseNormalMap(const float& use) const;
	void LoadMeshMaterial(const SMaterial& material) const;

	void LoadToShadowSpaceMatrix(const glm::mat4& matrix) const;
	void LoadShadowValues(const float& is, const float& distance, const float& shadow_map_size) const;

	void LoadClipPlane(const glm::vec4 clip_plane = glm::vec4(0, 1, 0, 100000)) const;

	void LoadUseBonesTransformation(const float& is) const;
	void LoadBoneTransform(const glm::mat4& transform, unsigned int id) const;
private:
	int location_TransformationMatrix;
	int location_ProjectionMatrix;
	int location_ViewMatrix;

	//Textures locations
	int location_ModelTexture;
	int location_UseNormalMap;
	int location_NormalMap;

	//Shadows Variables
	int location_ShadowVariables;
	int location_ShadowMap;
	int location_ToShadowMapSpace;

	//Material Locations
	int location_MaterialAmbient;
	int location_MaterialDiffuse;
	int location_MaterialSpecular;

	int location_IsInstancedRender;

	int location_ClipPlane;
	int location_ViewDistance;
	int location_UseFakeLighting;

	//Animations
	int location_UseBoneTransform;
	int location_Bones[MAX_BONES];

};