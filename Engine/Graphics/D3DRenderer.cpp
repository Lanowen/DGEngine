#include "d3drenderer.h"

D3DRenderer::D3DRenderer()
{
	this->m_device = 0;
	this->m_deviceContext = 0;

	m_viewPorts.reserve(20);

	bool result = this->CreateDevice();
	if (!result)
	{
		throw std::exception("Failed to create D3D Device");
	}	
}

//D3DRenderer::D3DRenderer(const D3DRenderer& other)
//{
//	m_viewPorts = other.m_viewPorts;
//	m_videoCardMemory = other.m_videoCardMemory;
//	memcpy(m_videoCardDescription,other.m_videoCardDescription, sizeof(other.m_videoCardDescription));
//	m_device = other.m_device;
//	m_deviceContext = other.m_deviceContext;
//}

D3DRenderer::~D3DRenderer()
{
	if (this->m_device)
	{
		this->m_device->Release();
		this->m_device = 0;
	}

	if (this->m_deviceContext)
	{
		this->m_deviceContext->Release();
		this->m_deviceContext = 0;
	}
}

bool D3DRenderer::CreateDevice()
{
	HRESULT result;
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		//D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &this->m_device, NULL, &this->m_deviceContext);

	if (FAILED(result))
	{
		return false;
	}
	IDXGIDevice* dxgiDevice = 0;
	IDXGIAdapter* dxgiAdapter =0;
	DXGI_ADAPTER_DESC adapterDesc;
	unsigned int stringLength;

	this->m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	result = dxgiAdapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	this->m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	int error = wcstombs_s(&stringLength, this->m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	dxgiAdapter->Release();
	dxgiAdapter = 0;

	return true;
}

ViewPort* D3DRenderer::CreateViewport(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	ViewPort* port = new ViewPort(this->m_device, screenWidth, screenHeight, vsync, hwnd, fullscreen, screenDepth, screenNear);

	m_viewPorts.push_back(port);

	m_deviceContext->RSSetViewports(m_viewPorts.size(), m_viewPorts[0]);

	return port;
}



ID3D11DeviceContext* D3DRenderer::GetDeviceContext()
{
	return this->m_deviceContext;
}

ID3D11Device* D3DRenderer::GetDevice()
{
	return this->m_device;
}


void D3DRenderer::GetVideoCardInfo(char * cardName, int& memory)
{
	strcpy_s(cardName, 128, this->m_videoCardDescription);
	memory = this->m_videoCardMemory;
	return;
}