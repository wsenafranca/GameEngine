#ifndef APP_LISTENER_H
#define APP_LISTENER_H

class AppListener
{
public:
	AppListener() = default;
	virtual ~AppListener() = default;
	virtual void onCreate() = 0;
	virtual void onUpdate(float dt) = 0;
	virtual void onDestroy() = 0;
};

#endif