#pragma once

#include <Windows.h>
#include "D3DRenderer.h"
#include "Camera.h"
#include "TextureShader.h"
#include "ColorShader.h"
#include <ResourceLoader.h>
#include "Model.h"
#include "IRenderable.hpp"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsSystem {
public:
	GraphicsSystem();
	GraphicsSystem(const GraphicsSystem&);
	~GraphicsSystem();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd, ResourceLoader*);
	void Shutdown();

	Model* LoadModel(Model* model, std::string modelName, std::wstring texture);
	Model* LoadModel(std::string modelName, std::wstring texture);

	void BeginScene();
	void Render(IRenderable* renderable);
	void EndScene();

	Camera* GetCamera(){ return m_Camera; }


	D3DRenderer *m_D3D;
	Camera *m_Camera;
	TextureShader* m_TextureShader;
	ColorShader* m_ColorShader;
	ResourceLoader* m_ResourceLoader;

private:
	Mat44 m_viewMatrix, m_projectionMatrix, m_worldMatrix, m_orthoMatrix;

public:
	PROPERTY_GET(Camera* camera, GetCamera);
};