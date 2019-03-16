#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
public:
	virtual ~Renderer() = default;
	virtual void begin() = 0;
	virtual void end() = 0;
};

#endif