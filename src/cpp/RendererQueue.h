#ifndef RENDERER_QUEUE_H
#define RENDERER_QUEUE_H

#include <Singleton.h>
#include <SpriteBatch.h>
#include <queue>
#include <AppListener.h>
#include <Math.h>

class RendererQueue : public AppListener {
public:
	SINGLETON(RendererQueue)

	void drawSprite(const TextureRegion &texture, float x, float y, float width, float height, float rotation, bool flipX, bool flipY, const b2Mat4& transform, const Color &color, int zOrder, const BlendFunc &blendFunc, const bool& blur);
	void onCreate() override;
	void onUpdate(float dt) override {}
	void onPostRender() override;
	void onDestroy() override;

	struct Context {
		BlendFunc blendFunc;
		int id, zOrder;
		TextureRegion texture;
		Color color;
		float x1, y1, x2, y2, x3, y3, x4, y4;
		bool flipX, flipY;
		bool blur;
	};
	struct ContextComparator {
		bool operator()(Context *c1, Context *c2) {
			if(c1->zOrder != c2->zOrder) {
				return c1->zOrder > c2->zOrder;
			}
			return c1->id > c2->id;
		}
	};

	RendererQueue() = default;
	~RendererQueue() = default;
	std::priority_queue<Context*, std::vector<Context*>, ContextComparator> q;
	SpriteBatch *batch;
};

#endif