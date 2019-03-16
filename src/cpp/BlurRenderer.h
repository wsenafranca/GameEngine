#ifndef BLUR_RENDERER_H
#define BLUR_RENDERER_H

#include <Renderer.h>
#include <OpenGL.h>
#include <Shader.h>
#include <Buffer.h>

class BlurRenderer : public Renderer {
public:
	BlurRenderer();
	virtual ~BlurRenderer();
	void begin();
	void end();
private:
	Shader shaderBlur, shaderBloom;
	Buffer *buffer;
	unsigned int fbo, rbo;
	unsigned int colorBuffers[2];

	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
};

#endif