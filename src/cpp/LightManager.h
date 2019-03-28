#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <Singleton.h>
#include <AppListener.h>
#include <RayHandler.h>
#include <DirectionalLight.h>
#include <PointLight.h>
#include <ConeLight.h>

class LightManager : public AppListener {
public:
	SINGLETON(LightManager)

	DirectionalLight* createDirectionalLight(int rays, const Color &color, float directionDegree);
	PointLight* createPointLight(int rays, const Color &color, float distance, float x, float y);
	ConeLight* createConeLight(int rays, const Color &color, float distance, float x, float y, float directionDegree, float coneDegree);
	void destroy(Light *light);

	void setAmbientLight(const Color &color);
	const Color& getAmbientLight() const;

	void onCreate() override;
	void onUpdate(float dt) override;
	void onPreRender() override;
	void onPostRender() override;
	void onDestroy() override;

private:
	LightManager() = default;
	~LightManager() = default;

	RayHandler *rayHandler;
	std::vector<Light*> lights;
};

#endif