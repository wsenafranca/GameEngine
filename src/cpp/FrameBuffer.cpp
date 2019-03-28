#include "FrameBuffer.h"
#include <stdexcept>

bool FrameBuffer::isDefaultFrameBufferInitialized = false;
int FrameBuffer::default_fbo = 0;

FrameBuffer::FrameBuffer() : fbo(0) {
}

FrameBuffer::~FrameBuffer() {
	if(fbo) destroy();
}

void FrameBuffer::create(int width, int height, GLenum internalformat, unsigned int numColorBuffers, bool depth, bool stencil) {
	if(!isDefaultFrameBufferInitialized) {
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &default_fbo);
		isDefaultFrameBufferInitialized = true;
	}
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	numTextures = numColorBuffers + depth + stencil;
	textures = new unsigned int[numTextures];

	unsigned int currentTexture;
	for(currentTexture = 0; currentTexture < numColorBuffers; currentTexture++) {
		glGenTextures(1, &textures[currentTexture]);
		glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+currentTexture, GL_TEXTURE_2D, textures[currentTexture], 0);
	}

	if(depth) {
		glGenRenderbuffers(1, &depth_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);

		glGenTextures(1, &textures[currentTexture]);
		glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[currentTexture], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);
		currentTexture++;
	}

	if(stencil) {
		glGenRenderbuffers(1, &stencil_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, stencil_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);

		glGenTextures(1, &textures[currentTexture]);
		glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textures[currentTexture], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil_rbo);
		currentTexture++;
	}

	if(numColorBuffers > 1) {
		setNumColorBuffers(numColorBuffers);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, default_fbo);
}

void FrameBuffer::destroy() {
	if(fbo) glDeleteFramebuffers(1, &fbo);
	if(depth_rbo) glDeleteRenderbuffers(1, &depth_rbo);
	if(stencil_rbo) glDeleteRenderbuffers(1, &stencil_rbo);
	glDeleteTextures(numTextures, textures);
	delete [] textures;
	fbo = 0;
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, default_fbo);
}

void FrameBuffer::begin() {
	bind();
	//setFrameBufferViewport();
}

void FrameBuffer::end() {
	unbind();
}

void FrameBuffer::setNumColorBuffers(unsigned int numColorBuffers) {
	unsigned int *attachments = new unsigned int[numColorBuffers];
	for(unsigned int i = 0; i < numColorBuffers; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(numColorBuffers, attachments);
	delete [] attachments;
}