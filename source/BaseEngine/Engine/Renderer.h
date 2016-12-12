#pragma once
namespace RendererDefines {

	enum {
		COLOR_BUFFER_BIT = 0x0001,
		DEPTH_BUFFER_BIT,
	};

}
class CRenderer 
{
public:
	virtual void Clear(int c) = 0;
	virtual void InitializeRenderer() = 0;
	virtual void CalculateFPS() = 0;
};