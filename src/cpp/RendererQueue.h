#ifndef RENDERER_QUEUE_H
#define RENDERER_QUEUE_H

#include <Singleton.h>
#include <SpriteBatch.h>
#include <ParticleBatch.h>
#include <queue>
#include <AppListener.h>
#include <Math.h>

class RendererQueue : public AppListener {
public:
	SINGLETON(RendererQueue)

	void drawSprite(const TextureRegion &texture, float x, float y, float width, float height, bool flipX, bool flipY, const b2Mat4& transform, const Color &color, int zOrder, const BlendFunc &blendFunc);
	void drawParticles(const TextureRegion &texture, unsigned int vbo, unsigned int numParticles, const b2Mat4& transform, const Color &color, int zOrder, const BlendFunc &blendFunc);
	void onCreate() override;
	void onUpdate(float dt) override;
	void onDestroy() override;
private:
	struct Context {
		Batch *batch;
		BlendFunc blendFunc;
		int id, zOrder;
		TextureRegion texture;
		Color color;
		virtual void draw() = 0;
	};
	struct ContextComparator {
		bool operator()(Context *c1, Context *c2) {
			if(c1->zOrder != c2->zOrder) {
				return c1->zOrder > c2->zOrder;
			}
			return c1->id > c2->id;
		}
	};

	struct SpriteContext : public Context {
		float x1, y1, x2, y2, x3, y3, x4, y4;
		bool flipX, flipY;
		void draw() override;
	};
	struct ParticleContext : public Context {
		unsigned int numParticles;
		unsigned int vbo;
		void draw() override;
		b2Mat4 transform;
	};
	RendererQueue() = default;
	~RendererQueue() = default;
	std::priority_queue<Context*, std::vector<Context*>, ContextComparator> q;
	SpriteBatch *spriteBatch;
	ParticleBatch *particlesBatch;
};

#endif