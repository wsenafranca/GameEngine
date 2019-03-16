// simulate particles from http://particle2dx.com

typedef struct sParticleInput {
	// input
	unsigned char emitterType; // 0 - Gravity 1 - Radius
	float particleLifespan; // life

	float gravityx; // gravity x
	float gravityy; // gravity y

	// start rgba
	unsigned char startColorRed;
	unsigned char startColorGreen;
	unsigned char startColorBlue;
	unsigned char startColorAlpha;

	// end rgba
	unsigned char finishColorRed;
	unsigned char finishColorGreen;
	unsigned char finishColorBlue;
	unsigned char finishColorAlpha;

	float startParticleSize; // initial particle size
	float finishParticleSize; // finish particle size
	
	float angle; // direction (radians)
	
	int rotatePerSecond;
	
	float rotationStart;
	float rotationEnd;
	
	float radialAcceleration;
	float tangentialAcceleration;
	
	float minRadius;
	float maxRadius;

	int speed;
	
} ParticleInput;

typedef struct sParticle {
	float x, y, z; // translate
	float sx, sy; // scale
	float angle; // rotate
	unsigned char r, g, b, a; // color

	float state;
	float particleLifespanVariance; // life variance

	// start variance rgba 0-255
	unsigned char startColorVarianceRed;
	unsigned char startColorVarianceGreen;
	unsigned char startColorVarianceBlue;
	unsigned char startColorVarianceAlpha;

	// end variance rbga 0-255
	unsigned char finishColorVarianceRed;
	unsigned char finishColorVarianceGreen;
	unsigned char finishColorVarianceBlue;
	unsigned char finishColorVarianceAlpha;

	float sourcePositionVariancex; // initial x position variance
	float sourcePositionVariancey; // initial x position variance

	float startParticleSizeVariance;
	float finishParticleSizeVariance;

	float angleVariance; // direction variance (radians)

	int rotatePerSecondVariance;

	float rotationStartVariance;
	float rotationEndVariance;

	float radialAccelVariance;
	float tangentialAccelVariance;

	float minRadiusVariance;
	float maxRadiusVariance;

	int speedVariance;
	unsigned char alive;

} Particle;

float progress(float state, float time) {
	return time == 0.0f ? 0.0f : min(state/time, 1.0f);
}

float interpolate(float Min, float Max, float progress) {
	return (Max - Min)*progress + Min;
}

unsigned char toByte(float r) {
	return (unsigned char) max(0, min((int)(r * 255.0f), 255));
}

float toFloat(unsigned char r) {
	return r/255.0f;
}

void kernel gravity(global Particle *particles, ParticleInput input, float delta) {
	Particle p = particles[get_global_id(0)];
	
	float life = max(0.0f, input.particleLifespan + p.particleLifespanVariance);
	p.state += delta;
	if(p.state >= life) {
		particles[get_global_id(0)].alive = false;
		return;
	}

	float r = progress(p.state, life);
		
	float speed = max(0, input.speed + p.speedVariance);
	float dirx = cos( ((input.angle + p.angleVariance)*M_PI)/180.0f );
	float diry = sin( ((input.angle + p.angleVariance)*M_PI)/180.0f );
	float dx = input.gravityx + dirx*speed;
	float dy = input.gravityy + diry*speed;
	p.x += dx*delta;
	p.y += dy*delta;
	p.z = 0;
	p.sx = interpolate( input.startParticleSize + p.startParticleSizeVariance, 
					input.finishParticleSize + p.finishParticleSizeVariance, r);
	p.sy = interpolate( input.startParticleSize + p.startParticleSizeVariance, 
						input.finishParticleSize + p.finishParticleSizeVariance, r);
	p.r = toByte(interpolate(toFloat(input.startColorRed + p.startColorVarianceRed), 
							 toFloat(input.finishColorRed + p.finishColorVarianceRed), r));
	p.g = toByte(interpolate(toFloat(input.startColorGreen + p.startColorVarianceGreen), 
							 toFloat(input.finishColorGreen + p.finishColorVarianceGreen), r));
	p.b = toByte(interpolate(toFloat(input.startColorBlue + p.startColorVarianceBlue), 
							 toFloat(input.finishColorBlue + p.finishColorVarianceBlue), r));
	p.a = toByte(interpolate(toFloat(input.startColorAlpha + p.startColorVarianceAlpha), 
							 toFloat(input.finishColorAlpha + p.finishColorVarianceAlpha), r));
	p.angle = 0.0f;

	particles[get_global_id(0)] = p;
}