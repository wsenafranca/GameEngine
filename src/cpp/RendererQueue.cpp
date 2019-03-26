#include "RendererQueue.h"
#include <cmath>

void RendererQueue::onCreate() {
	spriteBatch = new SpriteBatch();
	particlesBatch = new ParticleBatch();
}

void RendererQueue::drawSprite(const TextureRegion &texture, float x, float y, float width, float height, bool flipX, bool flipY, const b2Mat4& transform, const Color &color, int zOrder, const BlendFunc &blendFunc) {
	SpriteContext *context = new SpriteContext();
	context->batch = spriteBatch;
	context->texture = texture;
	context->color = color;
	context->zOrder = zOrder;
	context->id = q.size();
	context->flipX = flipX;
	context->flipY = flipY;
	context->blendFunc = blendFunc;
	context->x1 = (-width/2.0f + texture.offsetX)/texture.pixelPerUnit;
	context->y1 =  (height/2.0f + texture.offsetY)/texture.pixelPerUnit;
	context->x2 = (-width/2.0f + texture.offsetX)/texture.pixelPerUnit;
	context->y2 = (-height/2.0f + texture.offsetY)/texture.pixelPerUnit;
	context->x3 =  (width/2.0f + texture.offsetX)/texture.pixelPerUnit;
	context->y3 = (-height/2.0f + texture.offsetY)/texture.pixelPerUnit;
	context->x4 =  (width/2.0f + texture.offsetX)/texture.pixelPerUnit;
	context->y4 =  (height/2.0f + texture.offsetY)/texture.pixelPerUnit;

	b2Vec2 v;
	v.Set(context->x1, context->y1);
	v = b2Mul(transform, v);
	context->x1 = v.x + x;
	context->y1 = v.y + y;

	v.Set(context->x2, context->y2);
	v = b2Mul(transform, v);
	context->x2 = v.x + x;
	context->y2 = v.y + y;

	v.Set(context->x3, context->y3);
	v = b2Mul(transform, v);
	context->x3 = v.x + x;
	context->y3 = v.y + y;

	v.Set(context->x4, context->y4);
	v = b2Mul(transform, v);
	context->x4 = v.x + x;
	context->y4 = v.y + y;

	q.push(context);
	
}

void RendererQueue::drawParticles(const TextureRegion &texture, unsigned int vbo, unsigned int numParticles, const b2Mat4& transform, const Color &color, int zOrder, const BlendFunc &blendFunc) {
	ParticleContext *context = new ParticleContext();
	context->batch = particlesBatch;
	context->texture = texture;
	context->color = color;
	context->zOrder = zOrder;
	context->id = q.size();
	context->blendFunc = blendFunc;
	context->vbo = vbo;
	context->numParticles = numParticles;
	context->transform = transform;

	q.push(context);
}

void RendererQueue::onUpdate(float dt) {
	Batch *current = nullptr;
	while(!q.empty()) {
		Context *c = q.top();
		q.pop();
		if(current != c->batch) {
			if(current) current->end();
			current = c->batch;
			current->begin(*Camera::current());
		}
		current->setBlendFunc(c->blendFunc);
		c->draw();
	}
	if(current) current->end();
}

void RendererQueue::onDestroy() {
	delete spriteBatch;
	delete particlesBatch;
}

void RendererQueue::SpriteContext::draw() {
	SpriteBatch *batch = (SpriteBatch*) this->batch;
	batch->draw(texture, x1, y1, x2, y2, x3, y3, x4, y4, flipX, flipY, color);
}

void RendererQueue::ParticleContext::draw() {
	ParticleBatch *batch = (ParticleBatch*) this->batch;
	batch->draw(texture, vbo, numParticles, transform, color);
}