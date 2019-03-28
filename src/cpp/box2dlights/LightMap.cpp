#include "LightMap.h"

LightMap::LightMap(RayHandler *rayHandler, int fboWidth, int fboHeight) : 
				lightMapDrawingDisabled(false), fboWidth(fboWidth), fboHeight(fboHeight), rayHandler(rayHandler) 
{
	frameBuffer.create(fboWidth, fboHeight, GL_RGBA, 2);
	pingPongBuffer.create(fboWidth, fboHeight, GL_RGBA, 1);

	createLightMapMesh();
	shadowShader.add("glsl/shadow.vert", GL_VERTEX_SHADER);
	shadowShader.add("glsl/shadow.frag", GL_FRAGMENT_SHADER);
	shadowShader.create();

	withoutShadowShader.add("glsl/without_shadow.vert", GL_VERTEX_SHADER);
	withoutShadowShader.add("glsl/without_shadow.frag", GL_FRAGMENT_SHADER);
	withoutShadowShader.create();

	blurShader.add("glsl/blur.vert", GL_VERTEX_SHADER);
	blurShader.add("glsl/blur.frag", GL_FRAGMENT_SHADER);
	blurShader.create();

	diffuseShader.add("glsl/diffuse.vert", GL_VERTEX_SHADER);
	diffuseShader.add("glsl/diffuse.frag", GL_FRAGMENT_SHADER);
	diffuseShader.create();
}

LightMap::~LightMap() {
	frameBuffer.destroy();
	pingPongBuffer.destroy();
	shadowShader.destroy();
	withoutShadowShader.destroy();
	blurShader.destroy();
	diffuseShader.destroy();
}

void LightMap::renderScene() {
	glBindTexture(GL_TEXTURE_2D, frameBuffer.textures[0]);
	glBlendFuncSeparate(rayHandler->simpleBlendFunc.src, 
				rayHandler->simpleBlendFunc.dst, 
				rayHandler->simpleBlendFunc.src, 
				rayHandler->simpleBlendFunc.dst);
	withoutShadowShader.use();
	lightMapMesh.renderElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void LightMap::render() {
	bool needed = rayHandler->lightRenderedLastFrame > 0;
	if(lightMapDrawingDisabled) return;

	glBindTexture(GL_TEXTURE_2D, frameBuffer.textures[1]);
	if(rayHandler->shadows) {
		float r, g, b, a;
		rayHandler->ambientLight.toFloat(&r, &g, &b, &a);

		Shader *shader = &shadowShader;
		if(RayHandler::isDiffuse) {
			shader = &diffuseShader;
			shader->use();
			glBlendFuncSeparate(rayHandler->diffuseBlendFunc.src, 
				rayHandler->diffuseBlendFunc.dst, 
				rayHandler->diffuseBlendFunc.src, 
				rayHandler->diffuseBlendFunc.dst);
			glUniform4f(glGetUniformLocation(*shader, "ambient"), r, g, b, a);
		}
		else {
			shader->use();
			glBlendFuncSeparate(rayHandler->shadowBlendFunc.src, 
				rayHandler->shadowBlendFunc.dst, 
				rayHandler->shadowBlendFunc.src, 
				rayHandler->shadowBlendFunc.dst);
			glUniform4f(glGetUniformLocation(*shader, "ambient"), r*a, g*a, b*a, 1.0f-a);
		}
		lightMapMesh.renderElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else if(needed) {
		glBlendFuncSeparate(rayHandler->simpleBlendFunc.src, 
				rayHandler->simpleBlendFunc.dst, 
				rayHandler->simpleBlendFunc.src, 
				rayHandler->simpleBlendFunc.dst);
		withoutShadowShader.use();
		lightMapMesh.renderElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void LightMap::gaussianBlur() {
	glDisable(GL_BLEND);
	for(int i = 0; i < rayHandler->blurNum; i++) {
		glBindTexture(GL_TEXTURE_2D, frameBuffer.textures[1]);
		// horizontal
		pingPongBuffer.begin();
		{
			blurShader.use();
			glUniform2f(glGetUniformLocation(blurShader, "dir"), 1.0f, 0.0f);
			glUniform2f(glGetUniformLocation(blurShader, "fboSize"), fboWidth, fboHeight);
			glUniform1i(glGetUniformLocation(blurShader, "isDiffuse"), RayHandler::isDiffuse);
			lightMapMesh.renderElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		pingPongBuffer.end();

		glBindTexture(GL_TEXTURE_2D, pingPongBuffer.textures[0]);
		// vertical
		frameBuffer.begin();
		{
			blurShader.use();
			glUniform2f(glGetUniformLocation(blurShader, "dir"), 0.0f, 1.0f);
			glUniform2i(glGetUniformLocation(blurShader, "fboSize"), fboWidth, fboHeight);
			glUniform1i(glGetUniformLocation(blurShader, "isDiffuse"), RayHandler::isDiffuse);
			lightMapMesh.renderElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		frameBuffer.end();	
	}

	glEnable(GL_BLEND);
}

void LightMap::createLightMapMesh() {
	float vertices[] = {
		-1,  1, 0, 1,
		-1, -1, 0, 0,
		 1, -1, 1, 0,
		 1,  1, 1, 1
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	lightMapMesh.setVertices(vertices, 16, GL_STATIC_DRAW);
	lightMapMesh.setIndices(indices, 6, GL_STATIC_DRAW);
	lightMapMesh.setAttribute(0, Mesh::Attribute{2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)0});
	lightMapMesh.setAttribute(1, Mesh::Attribute{2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*2)});
}
