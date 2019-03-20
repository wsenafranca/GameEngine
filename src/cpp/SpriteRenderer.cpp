#include "SpriteRenderer.h"
#include <Sprite.h>
#include <glm/gtc/type_ptr.hpp>
#include <Application.h>

void SpriteRenderer::onCreate() {
	shader.add("glsl/base.vert", GL_VERTEX_SHADER);
	shader.add("glsl/base.frag", GL_FRAGMENT_SHADER);
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::onPreUpdate(float delta) {

}

void SpriteRenderer::onUpdate(float delta) {
}

void SpriteRenderer::onPostUpdate(float delta) {
	shader.use();
	glm::mat4 t = Camera::current()->combined()*node()->transform();
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_MVP"), 1, false, glm::value_ptr(t));
	glUniform4fv(glGetUniformLocation(shader, "u_color"), 1, glm::value_ptr(node()->color().toFloat()));
	glUniform1i(glGetUniformLocation(shader, "u_enableTexture"), node()->as<Sprite>()->texture());

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, node()->as<Sprite>()->texture());

	glEnable(GL_BLEND);
	glBlendFuncSeparate(node()->blendFunc().src, node()->blendFunc().dst, node()->blendFunc().src, node()->blendFunc().dst);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	glBlendFuncSeparate(BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst, BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst);
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void SpriteRenderer::onDestroy() {
	glDeleteBuffers(1, &vbo);
	shader.destroy();
}