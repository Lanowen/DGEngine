#include "graphicssystem.h"

GraphicsSystem::GraphicsSystem(HWND hwnd, ResourceLoader* loader){
	m_D3D = new D3DRenderer();
	//m_Camera = new Camera();
	m_TextureShader = new TextureShader();
	m_ColorShader = new ColorShader();
	m_FontShader = new FontShader();

	this->m_ResourceLoader = loader;

	if(!m_TextureShader || !m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd)){
		throw std::exception("Could not initialize the texture shader.");
	}
	TexturePass::Initialize(m_TextureShader);

	if(!m_ColorShader || !m_ColorShader->Initialize(this->m_D3D->GetDevice(), hwnd)){
		throw std::exception("Could not initialize the color shader.");
	}
	//TODO: Create colorPass shader type thing later sometime

	if(!m_FontShader || !m_FontShader->Initialize(this->m_D3D->GetDevice(), hwnd)){
		throw std::exception("Could not initialize the font shader.");
	}
	FontPass::Initialize(m_FontShader);
}

GraphicsSystem::GraphicsSystem(const GraphicsSystem& other){
	m_D3D = other.m_D3D;

	m_TextureShader = other.m_TextureShader;
	m_ColorShader = other.m_ColorShader;

	this->m_ResourceLoader = other.m_ResourceLoader;
}

GraphicsSystem::~GraphicsSystem(){
	if(m_D3D){
		delete m_D3D;
	}

	if(m_TextureShader){
		m_TextureShader->Shutdown();
		delete m_TextureShader;
	}

	
	if(m_ColorShader){
		m_ColorShader->Shutdown();
		delete m_ColorShader;
	}
}

ViewPort* GraphicsSystem::CreateViewport(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear){
	return m_D3D->CreateViewport(screenWidth, screenHeight, vsync, hwnd, fullscreen, screenDepth, screenNear);
}

Text* GraphicsSystem::CreateText(std::string str, std::string name){
	Text* text = new Text(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), str, name);	

	return text;
}

Bitmap* GraphicsSystem::CreateBitmap(std::wstring textureFilename, std::string name){
	Bitmap* bitmap = new Bitmap( this->m_D3D->GetDevice(), &textureFilename[0], name);

	return bitmap;
}

Model* GraphicsSystem::CreateModel(std::string modelName, std::wstring texture, std::string name){
	Model* model = new Model(this->m_D3D->GetDevice(), this->m_ResourceLoader->LoadModelData(&modelName[0]), &texture[0], name);

	return model;
}

