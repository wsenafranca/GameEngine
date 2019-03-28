#ifndef APP_LISTENER_H
#define APP_LISTENER_H

class AppListener
{
public:
	AppListener() = default;
	virtual ~AppListener() = default;
	virtual void onCreate() = 0;
	virtual void onPreUpdate(float dt) {}
	virtual void onUpdate(float dt) = 0;
	virtual void onPostUpdate(float dt) {}
	virtual void onPreRender() {}
	virtual void onRender() {}
	virtual void onPostRender() {}
	virtual void onDestroy() = 0;
};

#endif