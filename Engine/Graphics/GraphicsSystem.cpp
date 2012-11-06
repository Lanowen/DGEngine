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

	return model;
}

void GraphicsSystem::BeginScene(){
	//D3DXMATRIX m_viewMatrix, m_projectionMatrix, m_worldMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	this->m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	this->m_Camera->Render();

	this->m_Camera->GetViewMatrix(m_viewMatrix);
	this->m_D3D->GetWorldMatrix(m_worldMatrix);
	this->m_D3D->GetProjectionMatrix(m_projectionMatrix);
	this->m_D3D->GetOrthoMatrix(m_orthoMatrix);

	//D3DXMatrixRotationY(&m_worldMatrix, rotation);
}

bool GraphicsSystem::TextureShade(TexturedRenderObject* shadeObj){
	D3DXMATRIX translation, rotation;

	GameObject* go = reinterpret_cast<GameObject*>(shadeObj);

	D3DXMatrixTranslation(&translation, go->position.x, go->position.y, go->position.z);
	D3DXMatrixRotationQuaternion(&rotation, &go->rotation);

	return this->m_TextureShader->Render(this->m_D3D->GetDeviceContext(), shadeObj->GetIndexCount(), m_worldMatrix*rotation*translation, m_viewMatrix, m_projectionMatrix, shadeObj->GetTexture());
}

//bool GraphicsSystem::Render(Model* model)
//{
//	model->Render(this->m_D3D->GetDeviceContext());
//
//	D3DXMATRIX translation, rotation;
//
//	D3DXMatrixTranslation(&translation,  model->position.x, model->position.y, model->position.z);
//	D3DXMatrixRotationQuaternion(&rotation, &model->rotation);
//
//	bool result = this->m_TextureShader->Render(this->m_D3D->GetDeviceContext(), model->GetIndexCount(), m_worldMatrix*translation*rotation, m_viewMatrix, m_projectionMatrix, model->GetTexture());
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;	
//}

bool GraphicsSystem::Render(RenderObject* obj){
	obj->Render(this->m_D3D->GetDeviceContext());

	bool result = true;

	switch(obj->getShaderType()){
	case ShaderType::e_TextureShaded:
		result = TextureShade((TexturedRenderObject*)obj);
		break;
	case ShaderType::e_ColorShaded:
		//nothing for now
		break;
	case ShaderType::e_None:
	default:
		break;
	}

	if (!result){
		throw RenderException();

		return false;
	}

	return true;	
}

void GraphicsSystem::EndScene(){
	this->m_D3D->EndScene();
}

