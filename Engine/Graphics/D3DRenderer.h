#ifndef _D3DRENDERER_H_
#define _D3DRENDERER_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>
#include <D3DX10math.h>
#include "ViewPort.hpp"
#include <vector>

class D3DRenderer
{
public:
	D3DRenderer();
	//D3DRenderer(const D3DRenderer&);
	~D3DRenderer();

	//bool Initialize();
	//void Shutdown();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetVideoCardInfo(char*, int&);
	ViewPort* CreateViewport(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

private:
	bool CreateDevice();

private:
	std::vector<ViewPort*> m_viewPorts;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};

#endif