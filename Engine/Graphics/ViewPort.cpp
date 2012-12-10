#include "ViewPort.hpp"
#include "Camera.h"

ViewPort::ViewPort(ID3D11Device* device, int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) : m_device(device) {
	float fieldOfView, screenAspect;

	Width = (float)screenWidth;
	Height = (float)screenHeight;
	MinDepth = 0.0f;
	MaxDepth = 1.0f;
	TopLeftX = 0.0f;
	TopLeftY = 0.0f;

	device->GetImmediateContext(&this->m_deviceContext);
	this->m_deviceContext->RSSetViewports(1, this);

	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	D3DXMatrixPerspectiveFovLH(&this->m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	D3DXMatrixIdentity(&this->m_worldMatrix);

	D3DXMatrixOrthoLH(&this->m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	bool result;

	result = this->CreateSwapChain(Width, Height, hwnd);
	if (!result)
	{
		throw std::exception("Failed to create SwapChain");
	}

	result = this->CreateRenderTargetView();
	if (!result)
	{
		throw std::exception("Failed to create RenderTargetView");
	}
	
	result = this->CreateDepthBuffer(Width, Height);
	if (!result)
	{
		throw std::exception("Failed to create DepthBuffer");
	}

	result = this->CreateDepthStencilState();
	if (!result)
	{
		throw std::exception("Failed to create DepthStencilState");
	}

	result = this->CreateDepthDisabledStencilState();
	if (!result)
	{
		throw std::exception("Failed to create DepthDisabledStencilState");
	}

	result = this->CreateDepthStencilView();
	if (!result)
	{
		throw std::exception("Failed to create DepthStencilView");
	}

	result = this->CreateBlendState();
	if (!result)
	{
		throw std::exception("Failed to create BlendState");
	}
	
	result = this->CreateRasterizerState();
	if (!result)
	{
		throw std::exception("Failed to create RasterizerState");
	}

	TurnZBufferOn();
	TurnAlphaBlendingOn();

}

ViewPort::~ViewPort(){
	if (this->m_swapChain)
	{
		this->m_swapChain->SetFullscreenState(false, NULL);
	}
	
	if (this->m_rasterState)
	{
		this->m_rasterState->Release();
		this->m_rasterState = 0;
	}

	if (this->m_depthStencilView)
	{
		this->m_depthStencilView->Release();
		this->m_depthStencilView = 0;
	}

	if (this->m_depthStencilState)
	{
		this->m_depthStencilState->Release();
		this->m_depthStencilState = 0;
	}

	if (this->m_depthStencilBuffer)
	{
		this->m_depthStencilBuffer->Release();
		this->m_depthStencilBuffer = 0;
	}

	if (this->m_alphaEnabledBlendState)
	{
		this->m_alphaEnabledBlendState->Release();
		this->m_alphaEnabledBlendState = 0;
	}

	if (this->m_alphaDisabledBlendState)
	{
		this->m_alphaDisabledBlendState->Release();
		this->m_alphaDisabledBlendState = 0;
	}

	if (this->m_renderTargetView)
	{
		this->m_renderTargetView->Release();
		this->m_renderTargetView = 0;
	}

	if (this->m_swapChain)
	{
		this->m_swapChain->Release();
		this->m_swapChain = 0;
	}
}

bool ViewPort::CreateSwapChain(int screenWidth, int screenHeight, HWND hwnd)
{
	//Get refresh rate, display modes, and initialize the swap chain.
	HRESULT result;
	IDXGIDevice* dxgiDevice = 0;
	IDXGIFactory* dxgiFactory =0;
	IDXGIAdapter* dxgiAdapter =0;
	IDXGIOutput* dxgiAdapterOutput;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	this->m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	result = dxgiFactory->EnumAdapters(0, &dxgiAdapter);
	if (FAILED(result))
	{
		return false;
	}

	result = dxgiAdapter->EnumOutputs(0, &dxgiAdapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	result = dxgiAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	result = dxgiAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}



	for (i=0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	

	//Initialize the swap chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//Single buffer count
	swapChainDesc.BufferCount = 1;
	
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//Using regular 32bit back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (this->m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//Set the usage
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = hwnd;

	//Turn multisamping off. This setting is for MSAA, for our purposes we don't need it.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	//Set the scan line ordering
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Tells D3D to discard the back buffer after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Do not use advanced flags.
	swapChainDesc.Flags = 0;

	//Create the swap chain
	result = dxgiFactory->CreateSwapChain(this->m_device, &swapChainDesc, &this->m_swapChain);

	if (FAILED(result))
	{
		return false;
	}

	delete [] displayModeList;
	displayModeList = 0;

	dxgiAdapterOutput->Release();
	dxgiAdapterOutput = 0;

	dxgiAdapter->Release();
	dxgiAdapter = 0;

	dxgiFactory->Release();
	dxgiFactory = 0;

	return true;
}

bool ViewPort::CreateRenderTargetView()
{
	HRESULT result;
	ID3D11Texture2D* backBufferPtr;

	//Attach the back buffer to the swap chain
	result = this->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	//Create the target view
	result = this->m_device->CreateRenderTargetView(backBufferPtr, NULL, &this->m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	return true;
}

bool ViewPort::CreateDepthBuffer(int screenWidth, int screenHeight)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	//Initialize
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//Setup
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Create the texture
	result = this->m_device->CreateTexture2D(&depthBufferDesc, NULL, &this->m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool ViewPort::CreateDepthStencilState()
{
	//Depth stencil desc controls what type of depth test D3D will do.

	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	//Initialize
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//Setup
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//Operations if pixel is front facing
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//Operations if pixel is back facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = this->m_device->CreateDepthStencilState(&depthStencilDesc, &this->m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	this->m_deviceContext->OMSetDepthStencilState(this->m_depthStencilState, 1);

	return true;
}

bool ViewPort::CreateDepthStencilView()
{
	//Creates the depth stencil buffer. This tells D3D to use the depth buffer as a stencil texture
	HRESULT result;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	//Initialize
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//Setup
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create
	result = this->m_device->CreateDepthStencilView(this->m_depthStencilBuffer, &depthStencilViewDesc, &this->m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//Bind the target view and stencil buffer to the render pipeline
	this->m_deviceContext->OMSetRenderTargets(1, &this->m_renderTargetView, this->m_depthStencilView);

	return true;
}

bool ViewPort::CreateRasterizerState()
{
	//Override the rasterizer state, this gives us more control. (OPTIONAL)
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterDesc;

	//Setup
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//Create the rasterizer
	result = this->m_device->CreateRasterizerState(&rasterDesc, &this->m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	//Tell D3D to use this one instead of it's default one.
	this->m_deviceContext->RSSetState(this->m_rasterState);

	return true;
}


void ViewPort::TurnZBufferOn(){
	this->m_deviceContext->OMSetDepthStencilState(this->m_depthStencilState, 1);
}

void ViewPort::TurnZBufferOff(){
	this->m_deviceContext->OMSetDepthStencilState(this->m_depthDisabledStencilState, 1);
}

void ViewPort::TurnAlphaBlendingOn(){
	float blendFactor[4] = {0,0,0,0};

	this->m_deviceContext->OMSetBlendState(this->m_alphaEnabledBlendState, blendFactor, 0xffffffff);
}

void ViewPort::TurnAlphaBlendingOff(){
	float blendFactor[4] = {0,0,0,0};

	this->m_deviceContext->OMSetBlendState(this->m_alphaDisabledBlendState, blendFactor, 0xffffffff);
}

bool ViewPort::CreateBlendState(){
	HRESULT result;
	D3D11_BLEND_DESC blendStateDesc;

	ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));

	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;


	result = this->m_device->CreateBlendState(&blendStateDesc,&m_alphaEnabledBlendState);
	if(FAILED(result))
	{
		return false;
	}


	blendStateDesc.RenderTarget[0].BlendEnable = false;

	result = this->m_device->CreateBlendState(&blendStateDesc,&m_alphaDisabledBlendState);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool ViewPort::CreateDepthDisabledStencilState()
{
	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = this->m_device->CreateDepthStencilState(&depthDisabledStencilDesc,&m_depthDisabledStencilState);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

void ViewPort::Render(){
	BeginScene(0,0,0,0);
	Mat44 world, proj, ortho;
	world = m_worldMatrix;
	proj = m_projectionMatrix;
	ortho = m_orthoMatrix;

	m_Camera->Render(this, world, proj, ortho);
	EndScene();
}


void ViewPort::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	this->m_deviceContext->ClearRenderTargetView(this->m_renderTargetView, color);

	this->m_deviceContext->ClearDepthStencilView(this->m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void ViewPort::EndScene()
{
	if (this->m_vsync_enabled)
	{
		this->m_swapChain->Present(1, 0);
	}
	else
	{
		this->m_swapChain->Present(0, 0);
	}

	return;
}


void ViewPort::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = this->m_projectionMatrix;
	return;
}

void ViewPort::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = this->m_worldMatrix;
	return;
}

void ViewPort::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = this->m_orthoMatrix;
	return;
}

ID3D11DeviceContext* ViewPort::GetDeviceContext()
{
	return this->m_deviceContext;
}

ID3D11Device* ViewPort::GetDevice()
{
	return this->m_device;
}
