#ifndef BATCH_H
#define BATCH_H

#include <Camera.h>
#include <OpenGL.h>

class Batch {
public:
	Batch() : m_blur(0) {}
	~Batch() = default;
	virtual void begin(const Camera &camera) = 0;
	virtual void end() = 0;
	virtual void flush() = 0;

	void setBlendFunc(const BlendFunc &blendFunc) {
		glBlendFuncSeparate(m_blendFunc.src, m_blendFunc.dst, m_blendFunc.src, m_blendFunc.dst);
		if(m_blendFunc != blendFunc) {
			m_blendFunc = blendFunc;
			flush();
		}
	}
	const BlendFunc& getBlendFunc() const {return m_blendFunc;}

	void setBlur(bool blur) {
		if(m_blur != blur) {
			flush();
			m_blur = blur;
		}
	}
	bool isBlur() const {return m_blur;}

private:
	BlendFunc m_blendFunc;
	bool m_blur;
};

#endif