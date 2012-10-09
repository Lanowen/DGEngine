#ifndef _GRAPHICSSYSTEM_H_
#define _GRAPHICSSYSTEM_H_

#include <Windows.h>
#include "D3DRenderer.h"
#include "Camera.h"
#include "Model.h"
#include "TextureShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsSystem {
public:
	GraphicsSystem();
	GraphicsSystem(const GraphicsSystem&);
	~GraphicsSystem();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render(float rotation);

private:
	D3DRenderer *m_d3d;
	Camera *m_Camera;
	Model *m_Model;
	TextureShader* m_textureShader;
};

#endif