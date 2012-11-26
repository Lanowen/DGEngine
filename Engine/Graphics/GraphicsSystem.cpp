#include "graphicssystem.h"

GraphicsSystem::GraphicsSystem(){
	m_D3D = new D3DRenderer();
	m_Camera = new Camera();
	m_TextureShader = new TextureShader();
	m_ColorShader = new ColorShader;
}

GraphicsSystem::GraphicsSystem(const GraphicsSystem& other){
}

GraphicsSystem::~GraphicsSystem(){
	delete m_D3D;
	delete m_Camera;
	delete m_TextureShader;
}

bool GraphicsSystem::Initialize(int screenWidth, int screenHeight, HWND hwnd, ResourceLoader* loader){

	this->m_ResourceLoader = loader;


	if(!m_D3D || !m_D3D->Initialize(screenWidth, screenHeight, false, hwnd, false, 1000.0f, 0.1f)){
		return false;
	}

	if(!m_Camera){
		return false;
	}

	if(!m_TextureShader || !m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd)){
		MessageBox(hwnd, L"Could not initialize the texture shader.", L"Error", MB_OK);
		return false;
	}
	TexturePass::Initialize(m_TextureShader);

	if(!m_ColorShader || !m_ColorShader->Initialize(this->m_D3D->GetDevice(), hwnd)){
		MessageBox(hwnd, L"Could not initialize the color shader.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsSystem::Shutdown(){

	m_D3D->Shutdown();

	m_TextureShader->Shutdown();
	m_ColorShader->Shutdown();

	return;
}

Model* GraphicsSystem::LoadModel(std::string modelName, std::wstring texture){
	Model* model = new Model;
	if (!model)
	{
		return NULL;
	}

	bool result = model->Initialize(this->m_D3D->GetDevice(), this->m_ResourceLoader->LoadModelData(&modelName[0]), &texture[0]);
	if (!result)
	{
		//MessageBox(this->m_hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		delete model;
		return NULL;
	}

	return model;
}

Model* GraphicsSystem::LoadModel(Model* model, std::string modelName, std::wstring texture){
	bool result = model->Initialize(this->m_D3D->GetDevice(), this->m_ResourceLoader->LoadModelData(&modelName[0]), &texture[0]);
	if (!result)
	{
		//MessageBox(this->m_hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		delete model;
		return NULL;
	}

	model->setRenderTarget(m_D3D);

	return model;
}

void GraphicsSystem::BeginScene(){
	// Clear the buffers to begin the scene.
	this->m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	this->m_Camera->Render();

	this->m_Camera->GetViewMatrix(m_viewMatrix);
	this->m_D3D->GetWorldMatrix(m_worldMatrix);
	this->m_D3D->GetProjectionMatrix(m_projectionMatrix);
	this->m_D3D->GetOrthoMatrix(m_orthoMatrix);
}

void GraphicsSystem::Render(IRenderable* obj){
	obj->RenderToTarget(this->m_D3D->GetDeviceContext());
	bool result = obj->ShadeToTarget(this->m_D3D->GetDeviceContext(), m_worldMatrix, m_viewMatrix, m_projectionMatrix);

	//if (!result){
	//	throw RenderException();
	//}
}

void GraphicsSystem::EndScene(){
	this->m_D3D->EndScene();
}

