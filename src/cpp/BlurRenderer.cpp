#include "BlurRenderer.h"
#include <stdexcept>

BlurRenderer::BlurRenderer() {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
	glGenTextures(2, colorBuffers);
	for(unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, viewport[2], viewport[3], 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewport[2], viewport[3]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for(unsigned int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, viewport[2], viewport[3], 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error("Ping-pong Framebuffer not complete!");
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shaderBlur.add("glsl/blur.vert", GL_VERTEX_SHADER);
	shaderBlur.add("glsl/blur.frag", GL_FRAGMENT_SHADER);
	shaderBlur.create();

	shaderBloom.add("glsl/blur.vert", GL_VERTEX_SHADER);
	shaderBloom.add("glsl/bloom.frag", GL_FRAGMENT_SHADER);
	shaderBloom.create();

	std::vector<float> vertices {
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f
	};
	std::vector<unsigned int> indices {
		0, 1, 2,
		2, 3, 0
	};
	buffer = new Buffer(vertices, indices);
}

BlurRenderer::~BlurRenderer() {
	glDeleteBuffers(1, &fbo);
	glDeleteBuffers(1, &rbo);
	glDeleteBuffers(2, colorBuffers);
	glDeleteBuffers(2, pingpongFBO);
	glDeleteBuffers(2, pingpongColorbuffers);
	shaderBlur.destroy();
	shaderBloom.destroy();
	delete buffer;
}

void BlurRenderer::begin() {
	// 1. render scene into floating point framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BlurRenderer::end() {
	// 2. blur bright fragments with two-pass Gaussian Blur
	bool horizontal = true, firstIteration = true;
	unsigned int amount = 10;
	shaderBlur.use();
	for(unsigned int i = 0; i < amount; i++) {
		glUniform1i(glGetUniformLocation(shaderBlur, "u_horizontal"), horizontal);
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		glBindTexture(GL_TEXTURE_2D, firstIteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
		horizontal = !horizontal;
		buffer->bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		buffer->draw();
		if(firstIteration) {
			firstIteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderBloom.use();
	glUniform1i(glGetUniformLocation(shaderBloom, "u_bloom"), true);
	glUniform1f(glGetUniformLocation(shaderBloom, "u_exposure"), 1.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	buffer->bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	buffer->draw();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}