#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <cmath>
#include <algorithm>

enum AnimationMode {
	NO_REPETITION = 0,
	LOOP = 1,
	LOOP_REVERSE = 2
};

template<class T>
class Animation {
public:
	Animation(float duration, AnimationMode mode, const std::vector<T> &frames) : 
					duration(duration), frameDuration(duration / frames.size()), mode(mode), frames(frames) {}

	~Animation() {}

	T getFrame(float time) const {
		if(frames.size() == 1) {
			return frames[0];
		}

		int frame = round(time / frameDuration);
		switch(mode) {
			case LOOP:
				frame = frame % frames.size();
				break;
			case LOOP_REVERSE:
				break;
			default:
				frame = std::min(frame, (int) frames.size()-1);
				break;
		}

		return frames[frame];
	}

	float getDuration() const {
		return duration;
	}

	bool isEndAnimation(float time) const {
		return time > duration;
	}
private:
	float duration;
	float frameDuration;
	AnimationMode mode;
	std::vector<T> frames;
};

#endif