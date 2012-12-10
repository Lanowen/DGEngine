#pragma once

#include <Windows.h>
#include "D3DRenderer.h"
//#include "Camera.h"
#include "TextureShader.h"
#include "ColorShader.h"
#include <ResourceLoader.h>
#include "Model.h"
//#include "IRenderable.hpp"
#include "Scene.hpp"
#include "ViewPort.hpp"
#include "FontShader.h"
#include "Text.h"
#include "Bitmap.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsSystem {
public:
	GraphicsSystem(HWND hwnd, ResourceLoader* loader);
	GraphicsSystem(const GraphicsSystem&);
	~GraphicsSystem();

	ViewPort* CreateViewport(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	Text* CreateText(std::string text, std::string name);
	Bitmap* CreateBitmap(std::wstring textureFilename, std::string name);
	Model* CreateModel(std::string modelName, std::wstring texture, std::string name);

private:

	D3DRenderer *m_D3D;

	TextureShader* m_TextureShader;
	ColorShader* m_ColorShader;
	FontShader* m_FontShader;
	ResourceLoader* m_ResourceLoader;

};