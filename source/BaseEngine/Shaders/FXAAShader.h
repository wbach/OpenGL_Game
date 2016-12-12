#pragma once

#include "ShaderProgram.h"

class CFXAAShader : public CShaderProgram
{
public:
	void Init();
	void LoadFxaaSpanMax(const float &v) const;
	void LoadFxaaReduceMin(const float &v) const;
	void LoadFxaaReduceMul(const float &v) const;
	void LoadScreenSize(const glm::vec2& screen_size) const;

	void ConnectTextureUnits() const;
	void GetAllUniformLocations() override;
	void BindAttributes() override;
private:
	int location_FxaaSpanMax;
	int location_FxaaReduceMin;
	int location_FxaaReduceMul;
	int location_ScreenSize;

	int location_Texture;
};