#include "graphicssystem.h"

GraphicsSystem::GraphicsSystem(){
	m_d3d = new D3DRenderer();
	m_Camera = new Camera();
	m_Model = new Model();

	m_textureShader = new TextureShader();
}

GraphicsSystem::GraphicsSystem(const GraphicsSystem& other){
}

GraphicsSystem::~GraphicsSystem(){
	delete m_d3d;
	delete m_Camera;
	delete m_Model;
	delete m_textureShader;
}

bool GraphicsSystem::Initialize(int screenWidth, int screenHeight, HWND hwnd){

	//Testing

	if(!m_d3d || !m_d3d->Initialize(screenWidth, screenHeight, false, hwnd, false, 1000.0f, 0.1f)){
		return false;
	}

	if(!m_Camera){
		return false;
	}

	if(!m_Model || !m_Model->Initialize(m_d3d->GetDevice(), L"avatar.jpg", D3DXVECTOR3(0,0,3))){
	}

	if(!m_textureShader || !m_textureShader->Initialize(m_d3d->GetDevice(), hwnd)){
		return false;
	}

	return true;
}

void GraphicsSystem::Shutdown(){

	m_d3d->Shutdown();

	m_Model->Shutdown();

	m_textureShader->Shutdown();

	return;
}

bool GraphicsSystem::Frame(){

	//test camrea move
	static double x;
	x+=0.001;
	m_Camera->rotation = Quat::fromYawPitchRoll(0,0,(sin(x))*0.2);
	m_Camera->position = Vec3(0,0,(cos(x/4))*0.4);
	//it works

	return Render(0.0f);
}

bool GraphicsSystem::Render(float rotation){
	Mat44 viewMatrix, projMatrix, worldMatrix;
	bool result;

	m_d3d->BeginScene(0,0,0,1.0f);
	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_d3d->GetWorldMatrix(worldMatrix);
	m_d3d->GetProjectionMatrix(projMatrix);

	D3DXMatrixRotationY(&worldMatrix, rotation);

	m_Model->Render(m_d3d->GetDeviceContext());

	m_textureShader->Render(m_d3d->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projMatrix, m_Model->GetTexture());

	m_d3d->EndScene();
	
	return true;
}