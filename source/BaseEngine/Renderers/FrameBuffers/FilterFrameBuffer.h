#pragma once
#include "FrameBuffer.h"
class CFilterFrameBuffer  : public CFrameBuffer
{
public:
	virtual void Init(const glm::vec2 & size) override;

};