#pragma once

class COpenGLObject
{
public:
	virtual void OpenGLLoadingPass() = 0;
	// for for example instaced vbos which have to be done after loading model
	virtual void OpenGLPostLoadingPass() {};
};