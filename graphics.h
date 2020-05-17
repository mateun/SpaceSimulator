#pragma once
#include <Windows.h>


/**
* This class represents a graphics layer such as OpenGL or DirectX. 
* It is meant to be a rather small layer of abstraction over 
* the low level graphics apis. 
* Engine functionality may be built upon these abstractions here. 
*/
class GraphicsDevice {

public:
	GraphicsDevice(HWND h_wnd, int width, int height, bool fullscreen): h_wnd_(nullptr), width_(width), height_(height),
	                                                                    fullscreen_(fullscreen)
	{
	}

	virtual ~GraphicsDevice() {}

	virtual void clear_screen(float r, float g, float b, float a) = 0;
	virtual void present() = 0;


protected:
	HWND h_wnd_;
	int width_;
	int height_;
	bool fullscreen_;
};
