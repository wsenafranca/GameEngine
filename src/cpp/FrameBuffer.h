#ifndef FRAME_BUFFER
#define FRAME_BUFFER

#include <OpenGL.h>

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

	void create(int width, int height, GLenum internalformat, unsigned int numColorBuffers = 1, bool depth = false, bool stencil = false);
	void destroy();

	void bind();
	void unbind();

	void begin();
	void end();

	static void setNumColorBuffers(unsigned int numColorBuffers);

	unsigned int fbo, depth_rbo, stencil_rbo;
	unsigned int *textures;
	unsigned int numTextures;

	static bool isDefaultFrameBufferInitialized;
	static int default_fbo;
};

#endif