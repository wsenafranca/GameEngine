#include "SpriteBatch.h"
#include <cstdlib>
#include <cstdio>

SpriteBatch::SpriteBatch() {
	init(1000);
}

SpriteBatch::SpriteBatch(unsigned int size) {
	init(size);
}

SpriteBatch::~SpriteBatch() {
	free(vertices);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	shader.destroy();
}

void SpriteBatch::begin(const Camera &camera) {
	shader.use();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_Projection"), 1, false, camera.getProjection());
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_View"), 1, false, camera.getView());
}

void SpriteBatch::draw(const TextureRegion &texture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, bool flipX, bool flipY, const Color &color) {
	glEnable(GL_BLEND);
	glBlendFuncSeparate(getBlendFunc().src, getBlendFunc().dst, getBlendFunc().src, getBlendFunc().dst);
	if(texture.texture != currentTexture) {
		flush();
		currentTexture = texture.texture;
		if(currentTexture) {
			invTextureWidth = 1.0f/currentTexture->width();
			invTextureHeight = 1.0f/currentTexture->height();
			glUniform1i(glGetUniformLocation(shader, "u_enableTexture"), 1);
		}
		else {
			invTextureWidth = 0.0f;
			invTextureHeight = 0.0f;
			glUniform1i(glGetUniformLocation(shader, "u_enableTexture"), 0);
		}
	}
	else if(numVertices == maxVertices) {
		flush();
	}

	float u = texture.x*invTextureWidth;
	float v = texture.y*invTextureHeight;
	float u2 = (texture.x + texture.width)*invTextureWidth;
	float v2 = (texture.y + texture.height)*invTextureHeight;
	if(flipX) {
		std::swap(u, u2);
	}
	if(flipY) {
		std::swap(v, v2);
	}
	// top left
	vertices[numVertices++] = x1; // x
	vertices[numVertices++] = y1; // y
	vertices[numVertices++] = Color::pack(color);
	vertices[numVertices++] = u; // u
	vertices[numVertices++] = v; // v

	// bottom left
	vertices[numVertices++] = x2; // x
	vertices[numVertices++] = y2; // y
	vertices[numVertices++] = Color::pack(color);
	vertices[numVertices++] = u; // u
	vertices[numVertices++] = v2; // v

	// bottom right	
	vertices[numVertices++] = x3; // x
	vertices[numVertices++] = y3; // y
	vertices[numVertices++] = Color::pack(color);
	vertices[numVertices++] = u2; // u
	vertices[numVertices++] = v2; // v

	// top right
	vertices[numVertices++] = x4; // x
	vertices[numVertices++] = y4; // y
	vertices[numVertices++] = Color::pack(color);
	vertices[numVertices++] = u2; // u
	vertices[numVertices++] = v; // v

	numObjects++;
}

void SpriteBatch::end() {
	if(numVertices > 0) {
		flush();
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBlendFuncSeparate(BlendFunc::ALPHA_NON_PREMULTIPLIED.src, BlendFunc::ALPHA_NON_PREMULTIPLIED.dst, 
						BlendFunc::ALPHA_NON_PREMULTIPLIED.src, BlendFunc::ALPHA_NON_PREMULTIPLIED.dst);
	currentTexture = 0;
}

void SpriteBatch::init(unsigned int size) {
	maxVertices = size*4*5;
	numVertices = 0;
	numObjects = 0;
	currentTexture = 0;

	vertices = (float*) malloc(sizeof(float)*maxVertices);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*maxVertices, nullptr, GL_DYNAMIC_DRAW);

	numIndices = size*6;
	unsigned int *indices = (unsigned int*) malloc(sizeof(unsigned int)*numIndices);
	for(int i = 0, j = 0; i < numIndices; i+=6, j+=4) {
		indices[i    ] = j;
		indices[i + 1] = (j + 1);
		indices[i + 2] = (j + 2);
		indices[i + 3] = (j + 2);
		indices[i + 4] = (j + 3);
		indices[i + 5] = j;
	}
	glGenBuffers(1, &ebo);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), indices, GL_STATIC_DRAW);
 	free(indices);

 	glBindVertexArray(0);
 	glBindBuffer(GL_ARRAY_BUFFER, 0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.add("glsl/default.vert", GL_VERTEX_SHADER);
	shader.add("glsl/default.frag", GL_FRAGMENT_SHADER);
	shader.create();
}

void SpriteBatch::flush() {
	if(numVertices == 0) return;
	glBindTexture(GL_TEXTURE_2D, currentTexture ? *currentTexture : 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*numVertices, vertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)0);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GLfloat)*5, (void*)(sizeof(GLfloat)*2));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)(sizeof(GLfloat)*3));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);

	glDrawElements(GL_TRIANGLES, 6*numObjects, GL_UNSIGNED_INT, 0);

 	glBindBuffer(GL_ARRAY_BUFFER, 0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	numVertices = 0;
	numObjects = 0;
}

