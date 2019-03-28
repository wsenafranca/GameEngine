#include "RayHandler.h"
#include "Light.h"
#include "LightMap.h"

bool RayHandler::gammaCorrection = false;
float RayHandler::gammaCorrectionParameter = 1.0f;
bool RayHandler::isDiffuse = false;

RayHandler::RayHandler(b2World *world, int viewportWidth, int viewportHeight) :  
							diffuseBlendFunc(BlendFunc{GL_DST_COLOR, GL_ZERO}), 
							shadowBlendFunc(BlendFunc{GL_ONE, GL_ONE_MINUS_SRC_ALPHA}), 
							simpleBlendFunc(BlendFunc{GL_SRC_ALPHA, GL_ONE}),
							culling(false), shadows(true), blur(true),
							blurNum(1), 
							viewportX(0), viewportY(0),
							lightRenderedLastFrame(0),
							world(world)
{
	ambientLight = Color(0, 0, 0, 255);
	resizeFBO(viewportWidth, viewportHeight);
	shader.add("glsl/light.vert", GL_VERTEX_SHADER);
	shader.add("glsl/light.frag", GL_FRAGMENT_SHADER);
	shader.create();
}

RayHandler::~RayHandler() {
	shader.destroy();
	delete lightMap;
}

void RayHandler::resizeFBO(int fboWidth, int fboHeight) {
	lightMap = new LightMap(this, fboWidth, fboHeight);
}

void RayHandler::setCombinedMatrix(const Camera &camera) {
	setCombinedMatrix(
		camera.getCombined(),
		camera.getPosition().x,
		camera.getPosition().y,
		camera.getViewport().x*camera.getZoom(),
		camera.getViewport().y*camera.getZoom());
}

void RayHandler::setCombinedMatrix(const b2Mat4 &combined, float x, float y, float viewPortWidth, float viewPortHeight) {
	this->combined = combined;
	float halfViewPortWidth = viewPortWidth * 0.5f;
	x1 = x - halfViewPortWidth;
	x2 = x + halfViewPortWidth;

	float halfViewPortHeight = viewPortHeight * 0.5f;
	y1 = y - halfViewPortHeight;
	y2 = y + halfViewPortHeight;
}

bool RayHandler::intersect(float x, float y, float radius) {
	return (x1 < (x + radius) && x2 > (x - radius) && y1 < (y + radius) && y2 > (y - radius));
}

void RayHandler::update() {
	for (Light *light : lightList) {
		light->update();
	}
}

void RayHandler::begin() {
	lightRenderedLastFrame = 1;
	lightMap->frameBuffer.begin();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDepthMask(false);
}

void RayHandler::end() {
	lightMap->frameBuffer.end();
	lightMap->renderScene();
	if (shadows || blur) {
		lightMap->frameBuffer.begin();
		glEnable(GL_BLEND);
		glBlendFuncSeparate(simpleBlendFunc.src, simpleBlendFunc.dst, simpleBlendFunc.src, simpleBlendFunc.dst);
	}

	render();

	if (shadows || blur) {
		lightMap->frameBuffer.end();
		bool needed = lightRenderedLastFrame > 0;
		if (needed && blur) {
			lightMap->gaussianBlur();
		}
	}
	lightMap->render();
	glUseProgram(0);
}

void RayHandler::render() {
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_projTrans"), 1, false, combined);
	glUniform1i(glGetUniformLocation(shader, "isGammaCorrection"), gammaCorrection);
	for (Light *light : lightList) {
		light->render();
	};
}

bool RayHandler::pointAtLight(float x, float y) {
	for (Light *light : lightList) {
		if (light->contains(x, y)) return true;
	}
	return false;
}

bool RayHandler::pointAtShadow(float x, float y) {
	for (Light *light : lightList) {
		if (light->contains(x, y)) return false;
	}
	return true;
}

void RayHandler::setCulling(bool culling) {
	this->culling = culling;
}

void RayHandler::setBlur(bool blur) {
	this->blur = blur;
}

void RayHandler::setBlurNum(int blurNum) {
	this->blurNum = blurNum;
}

void RayHandler::setShadows(bool shadows) {
	this->shadows = shadows;
}

void RayHandler::setAmbientLight(float ambientLight) {
	this->ambientLight.a = std::clamp((int) ambientLight, 0, 255);
}

void RayHandler::setAmbientLight(float r, float g, float b, float a) {
	ambientLight.set(r, g, b, a);
}

void RayHandler::setAmbientLight(const Color &ambientLightColor) {
	ambientLight = ambientLightColor;
}

void RayHandler::setWorld(b2World *world) {
	this->world = world;
}

void RayHandler::setLightMapRendering(bool isAutomatic) {
	lightMap->lightMapDrawingDisabled = !isAutomatic;
}

unsigned int RayHandler::getLightMapTexture() {
	return lightMap->frameBuffer.textures[0];
}

const FrameBuffer& RayHandler::getLightMapBuffer() const {
	return lightMap->frameBuffer;
}