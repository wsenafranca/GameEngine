#ifndef RAY_HANDLER_H
#define RAY_HANDLER_H

#include <BlendFunc.h>
#include <vector>
#include <Math.h>
#include <Color.h>
#include <Shader.h>
#include <Box2D/Box2D.h>
#include <Camera.h>
#include <OpenGL.h>
#include <FrameBuffer.h>

#define GAMMA_COR 0.625f

class Light;
class LightMap;

class RayHandler {
public:
	RayHandler(b2World *world, int viewportWidth, int viewportHeight);
	~RayHandler();
	
	void resizeFBO(int fboWidth, int fboHeight);
	void setCombinedMatrix(const Camera &camera);
	void setCombinedMatrix(const b2Mat4 &combined, float x, float y, float viewPortWidth, float viewPortHeight);
	bool intersect(float x, float y, float radius);
	
	void update();
	void begin();
	void render();
	void end();

	bool pointAtLight(float x, float y);
	bool pointAtShadow(float x, float y);

	void setCulling(bool culling);
	void setBlur(bool blur);
	void setBlurNum(int blurNum);
	void setShadows(bool shadows);

	void setAmbientLight(float ambientLight);
	void setAmbientLight(float r, float g, float b, float a);
	void setAmbientLight(const Color &ambientLightColor);

	void setWorld(b2World *world);

	void setLightMapRendering(bool isAutomatic);
	unsigned int getLightMapTexture();

	const FrameBuffer& getLightMapBuffer() const;

	static bool gammaCorrection;
	static float gammaCorrectionParameter;
	static bool isDiffuse;

	BlendFunc diffuseBlendFunc, shadowBlendFunc, simpleBlendFunc;
	b2Mat4 combined;
	Color ambientLight;

	LightMap *lightMap;

	std::vector<Light*> lightList;
	std::vector<Light*> disabledLights;

	Shader shader;

	bool culling, shadows, blur;
	int blurNum;

	int viewportX, viewportY, viewportWidth, viewportHeight;

	int lightRenderedLastFrame;

	float x1, x2, y1, y2;

	b2World *world;
private:
	
};

#endif