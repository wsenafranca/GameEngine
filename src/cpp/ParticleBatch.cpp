#include "ParticleBatch.h"
#include <Particle.h>

ParticleBatch::ParticleBatch() {
	shader.add("glsl/particle.vert", GL_VERTEX_SHADER);
	shader.add("glsl/particle.frag", GL_FRAGMENT_SHADER);
	shader.create();

	static const GLfloat vertices[] = {
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

ParticleBatch::~ParticleBatch() {
	glDeleteBuffers(1, &vbo);
	shader.destroy();
}

void ParticleBatch::begin(const Camera &camera) {
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_Projection"), 1, false, camera.getProjection());
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_View"), 1, false, camera.getView());

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
}

void ParticleBatch::draw(const TextureRegion &texture, unsigned int vbo, unsigned int numParticles, const b2Mat4 &transform, const Color &color) {
	glEnable(GL_BLEND);
	glBlendFuncSeparate(getBlendFunc().src, getBlendFunc().dst,getBlendFunc().src, getBlendFunc().dst);
	
	glBindTexture(GL_TEXTURE_2D, *texture.texture);
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_Model"), 1, false, transform);
	glUniform1i(glGetUniformLocation(shader, "u_enableTexture"), texture.texture != 0);
	glUniform4f(glGetUniformLocation(shader, "u_color"), color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);	
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(GLfloat)*4));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numParticles);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void ParticleBatch::end() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBlendFuncSeparate(BlendFunc::ALPHA_NON_PREMULTIPLIED.src, BlendFunc::ALPHA_NON_PREMULTIPLIED.dst, 
						BlendFunc::ALPHA_NON_PREMULTIPLIED.src, BlendFunc::ALPHA_NON_PREMULTIPLIED.dst);
}