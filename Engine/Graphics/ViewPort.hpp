#pragma once
#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>
#include <D3DX10math.h>
#include <EMath.hpp>

class Camera;

class ViewPort : public D3D11_VIEWPORT {
	friend class D3DRenderer;
private:
	ViewPort(ID3D11Device* device, int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
public:
	//ViewPort(const ViewPort& other){}
	virtual ~ViewPort();

	void setCamera(Camera* camera) { m_Camera = camera;}

	void Render();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnAlphaBlendingOn();
	void TurnAlphaBlendingOff();

private:
	HWND m_hwnd;
	void BeginScene(float, float, float, float);
	void EndScene();

	bool CreateSwapChain(int, int, HWND);
	bool CreateRenderTargetView();
	bool CreateDepthBuffer(int, int);
	bool CreateDepthStencilState();
	bool CreateDepthStencilView();
	bool CreateRasterizerState();
	bool CreateDepthDisabledStencilState();
	bool CreateBlendState();

private:
	Camera* m_Camera;

	bool m_vsync_enabled;

	ID3D11Device* m_device;
	IDXGISwapChain* m_swapChain;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11BlendState* m_alphaEnabledBlendState;
	ID3D11BlendState* m_alphaDisabledBlendState;

	Mat44 m_projectionMatrix;
	Mat44 m_worldMatrix;
	Mat44 m_orthoMatrix;

};