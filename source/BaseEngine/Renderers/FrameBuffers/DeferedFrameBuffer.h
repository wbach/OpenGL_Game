#pragma once
#include "FrameBuffer.h"
class CDefferedFrameBuffer : public CFrameBuffer
{
public:
	void Init(const glm::vec2& size) override;
};