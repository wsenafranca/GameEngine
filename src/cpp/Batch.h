#ifndef BATCH_H
#define BATCH_H

#include <Camera.h>

class Batch {
public:
	Batch() = default;
	~Batch() = default;
	virtual void begin(const Camera &camera) = 0;
	virtual void end() = 0;
	void setBlendFunc(const BlendFunc &blendFunc) {m_blendFunc = blendFunc;}
	const BlendFunc& getBlendFunc() const {return m_blendFunc;}

private:
	BlendFunc m_blendFunc;
};

#endif