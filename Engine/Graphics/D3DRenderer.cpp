#include "d3drenderer.h"

D3DRenderer::D3DRenderer()
{
	this->m_swapChain = 0;
	this->m_device = 0;
	this->m_deviceContext = 0;
	this->m_renderTargetView = 0;
	this->m_depthStencilBuffer = 0;
	this->m_depthStencilState = 0;
	this->m_depthStencilView = 0;
	this->m_rasterState = 0;
}

D3DRenderer::D3DRenderer(const D3DRenderer& other)
{
}

D3DRenderer::~D3DRenderer()
{
}

bool D3DRenderer::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	bool result;
	this->m_vsync_enabled = vsync;

	result = this->CreateDevice();
	if (!result)
	{
		return false;
	}

	result = this->CreateSwapChain(screenWidth, screenHeight, hwnd);
	if (!result)
	{
		return false;
	}

	result = this->CreateRenderTargetView();
	if (!result)
	{
		return false;
	}
	
	result = this->CreateDepthBuffer(screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	result = this->CreateDepthStencilState();
	if (!result)
	{
		return false;
	}

	result = this->CreateDepthStencilView();
	if (!result)
	{
		return false;
	}
	
	/*result = this->CreateRasterizerState();
	if (!result)
	{
		return false;
	}*/

	result = this->CreateViewport(screenWidth, screenHeight, screenDepth, screenNear);
	if (!result)
	{
		return false;
	}
	
	return true;
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

	return true;
}

bool D3DRenderer::CreateSwapChain(int screenWidth, int screenHeight, HWND hwnd)
{
	//Get refresh rate, display modes, and initialize the swap chain.
	HRESULT result;
	IDXGIDevice* dxgiDevice = 0;
	IDXGIFactory* dxgiFactory =0;
	IDXGIAdapter* dxgiAdapter =0;
	IDXGIOutput* dxgiAdapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
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

	result = dxgiAdapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	this->m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, this->m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
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

bool D3DRenderer::CreateRenderTargetView()
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

bool D3DRenderer::CreateDepthBuffer(int screenWidth, int screenHeight)
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

bool D3DRenderer::CreateDepthStencilState()
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

bool D3DRenderer::CreateDepthStencilView()
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

bool D3DRenderer::CreateRasterizerState()
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

bool D3DRenderer::CreateViewport(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	this->m_deviceContext->RSSetViewports(1, &viewport);

	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	D3DXMatrixPerspectiveFovLH(&this->m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	D3DXMatrixIdentity(&this->m_worldMatrix);

	D3DXMatrixOrthoLH(&this->m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3DRenderer::Shutdown()
{
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

	if (this->m_renderTargetView)
	{
		this->m_renderTargetView->Release();
		this->m_renderTargetView = 0;
	}

	if (this->m_deviceContext)
	{
		this->m_deviceContext->Release();
		this->m_deviceContext = 0;
	}

	if (this->m_device)
	{
		this->m_device->Release();
		this->m_device = 0;
	}

	if (this->m_swapChain)
	{
		this->m_swapChain->Release();
		this->m_swapChain = 0;
	}

	return;
}

void D3DRenderer::BeginScene(float red, float green, float blue, float alpha)
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

void D3DRenderer::EndScene()
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

ID3D11Device* D3DRenderer::GetDevice()
{
	return this->m_device;
}

ID3D11DeviceContext* D3DRenderer::GetDeviceContext()
{
	return this->m_deviceContext;
}

void D3DRenderer::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = this->m_projectionMatrix;
	return;
}

void D3DRenderer::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = this->m_worldMatrix;
	return;
}

void D3DRenderer::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = this->m_orthoMatrix;
	return;
}

void D3DRenderer::GetVideoCardInfo(char * cardName, int& memory)
{
	strcpy_s(cardName, 128, this->m_videoCardDescription);
	memory = this->m_videoCardMemory;
	return;
}