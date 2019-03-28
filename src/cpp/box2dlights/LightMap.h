#ifndef LIGHT_MAP_H
#define LIGHT_MAP_H

#include <RayHandler.h>
#include <Shader.h>
#include <Mesh.h>
#include <Texture.h>
#include <FrameBuffer.h>

class LightMap {
public:
	LightMap(RayHandler *rayHandler, int fboWidth, int fboHeight);
	~LightMap();

	void renderScene();
	void render();
	void gaussianBlur();
	void createLightMapMesh();

	bool lightMapDrawingDisabled;

	FrameBuffer frameBuffer, pingPongBuffer;
	int fboWidth, fboHeight;
private:
	Shader shadowShader;
	Mesh lightMapMesh;

	RayHandler *rayHandler;
	Shader withoutShadowShader;
	Shader blurShader;
	Shader diffuseShader;
};

#endif