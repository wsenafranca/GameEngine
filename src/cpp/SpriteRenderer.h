#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <Component.h>
#include <Shader.h>
#include <Camera.h>

class SpriteRenderer : public Component {
	void onCreate() override;
	void onPreUpdate(float delta) override;
	void onUpdate(float delta) override;
	void onPostUpdate(float delta) override;
	void onDestroy() override;
private:
	Shader shader;
	unsigned int vbo;
};

#endif