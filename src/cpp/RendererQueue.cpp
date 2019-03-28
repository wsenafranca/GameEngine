#include "RendererQueue.h"
#include <cmath>

void RendererQueue::onCreate() {
	batch = new SpriteBatch();
}

void RendererQueue::drawSprite(const TextureRegion &texture, float x, float y, float width, float height, float rotation, bool flipX, bool flipY, const b2Mat4& transform, const Color &color, int zOrder, const BlendFunc &blendFunc, const bool& blur) {
	Context *context = new Context();
	context->texture = texture;
	context->color = color;
	context->zOrder = zOrder;
	context->id = q.size();
	context->flipX = flipX;
	context->flipY = flipY;
	context->blendFunc = blendFunc;
	context->blur = blur;
	
	float x1 = (-width/2.0f + texture.offsetX + x);
	float y1 =  (height/2.0f + texture.offsetY + y);
	float x2 = (-width/2.0f + texture.offsetX + x);
	float y2 = (-height/2.0f + texture.offsetY + y);
	float x3 =  (width/2.0f + texture.offsetX + x);
	float y3 = (-height/2.0f + texture.offsetY + y);
	float x4 =  (width/2.0f + texture.offsetX + x);
	float y4 =  (height/2.0f + texture.offsetY + y);

	if(rotation != 0) {
		float c = cosf(rotation);
		float s = sinf(rotation);

		context->x1 = c*x1 - s*y1;
		context->y1 = s*x1 + c*y1;

		context->x2 = c*x2 - s*y2;
		context->y2 = s*x2 + c*y2;

		context->x3 = c*x3 - s*y3;
		context->y3 = s*x3 + c*y3;

		context->x4 = c*x4 - s*y4;
		context->y4 = s*x4 + c*y4;
	}
	else {
		context->x1 = x1;
		context->y1 = y1;

		context->x2 = x2;
		context->y2 = y2;

		context->x3 = x3;
		context->y3 = y3;

		context->x4 = x4;
		context->y4 = y4;
	}

	context->x1 /= texture.pixelPerUnit;
	context->y1 /= texture.pixelPerUnit;
	context->x2 /= texture.pixelPerUnit;
	context->y2 /= texture.pixelPerUnit;
	context->x3 /= texture.pixelPerUnit;
	context->y3 /= texture.pixelPerUnit;
	context->x4 /= texture.pixelPerUnit;
	context->y4 /= texture.pixelPerUnit;

	b2Vec2 v;
	v.Set(context->x1, context->y1);
	v = b2Mul(transform, v);
	context->x1 = v.x;
	context->y1 = v.y;

	v.Set(context->x2, context->y2);
	v = b2Mul(transform, v);
	context->x2 = v.x;
	context->y2 = v.y;

	v.Set(context->x3, context->y3);
	v = b2Mul(transform, v);
	context->x3 = v.x;
	context->y3 = v.y;

	v.Set(context->x4, context->y4);
	v = b2Mul(transform, v);
	context->x4 = v.x;
	context->y4 = v.y;

	q.push(context);
	
}

void RendererQueue::onPostRender() {
	batch->begin(*Camera::current());
	while(!q.empty()) {
		Context *c = q.top();
		q.pop();
		batch->setBlendFunc(c->blendFunc);
		batch->setBlur(c->blur);
		batch->draw(c->texture, 
			c->x1, c->y1, 
			c->x2, c->y2, 
			c->x3, c->y3, 
			c->x4, c->y4, 
			c->flipX, c->flipY, 
			c->color);
	}
	batch->end();
}

void RendererQueue::onDestroy() {
	delete batch;
}
