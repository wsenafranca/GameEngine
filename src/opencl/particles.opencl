// simulate particles from http://particle2dx.com based on Cocos2Dx

inline static float RANDOM_M11(unsigned int *seed) {
	*seed = *seed * 134775813 + 1;
	union {
		unsigned int d;
		float f;
	} u;
	u.d = (((unsigned int)(*seed) & 0x7fff) << 8) | 0x40000000;
	return u.f - 3.0f;
}

inline static float angle(float x, float y) {
	return acos(clamp(dot(x, y), -1.0f, 1.0f));
}

inline static void normalize_point(float x, float y, float *outX, float *outY)
{
    float n = x * x + y * y;
    // Already normalized.
    if (n == 1.0f)
        return;
    
    n = sqrt(n);
    // Too close to zero.
    if (n < 1E-12)
        return;
    
    n = 1.0f / n;
    *outX = x * n;
    *outY = y * n;
}

void swap(float *x, float *y) {
	float tmp = *x;
	*x = *y;
	*y = tmp;
}

__constant unsigned char MODE_GRAVITY = 0;
__constant unsigned char MODE_RADIUS = 1;

__constant unsigned char POSITION_TYPE_FREE = 0;
__constant unsigned char POSITION_TYPE_RELATIVE_POS = 1;
__constant unsigned char POSITION_TYPE_GROUPED = 2;

typedef struct Vec2 {
	float x, y;
} vec2;

typedef struct Vec4 {
	float x, y, z, w;
} vec4;

typedef struct sParticleData {
	vec2 position;

	unsigned char isAutoRemoveOnFinish;
	float elapsed;

	struct {
		vec2 gravity;
		float speed;
		float speedVar;
		float tangentialVar;
		float tangentialAccel;
		float tangentialAccelVar;
		float radialAccel;
		float radialAccelVar;
		unsigned char rotationIsDir;
	} gravityMode;

	struct {
		float startRadius;
		float startRadiusVar;
		float endRadius;
		float endRadiusVar;
		float rotatePerSecond;
		float rotatePerSecondVar;
	} radiusMode;

	float emitCounter;
	int particleCount;

	float duration; // -1 means inifinite
	vec2 sourcePosition;
	vec2 posVar;
	float life;
	float lifeVar;
	float angle;
	float angleVar;

	unsigned char emitterMode;

	float startSize;
	float startSizeVar;
	float endSize;
	float endSizeVar;
	vec4 startColor;
	vec4 startColorVar;
	vec4 endColor;
	vec4 endColorVar;
	float startSpin;
	float startSpinVar;
	float endSpin;
	float endSpinVar;
	float emissionRate;

	int totalParticles;

	unsigned char opacityMotifyRGB;
	int yCoordFlipped;

	unsigned char positionType;
	unsigned char sourcePositionCompatible;
} ParticleData;

typedef struct sParticle {
	float x;
	float y;
	float size;
	float rotRad;
	float colorR, colorG, colorB, colorA;

	float posx;
	float posy;
	float startPosX;
	float startPosY;

	float deltaColorR;
	float deltaColorG;
	float deltaColorB;
	float deltaColorA;

	float deltaSize;
	float rotation;
	float deltaRotation;
	float timeToLive;

	struct {
		float dirX;
		float dirY;
		float radialAccel;
		float tangentialAccel;
	} gravityMode;

	struct {
		float angle;
		float degreesPerSecond;
		float radius;
		float deltaRadius;
	} radiusMode;
} Particle;

void kernel addParticles(global Particle *particles, global unsigned int *randomSeed, const ParticleData data, const unsigned int start) {
	unsigned int i = start + get_global_id(0);
	
	unsigned int RANDSEED = randomSeed[i];
	float timeToLive = max(0.0f, data.life + data.lifeVar*RANDOM_M11(&RANDSEED));

	particles[i].timeToLive = timeToLive;

	particles[i].posx = data.sourcePosition.x + data.posVar.x*RANDOM_M11(&RANDSEED);
	particles[i].posy = data.sourcePosition.y + data.posVar.y*RANDOM_M11(&RANDSEED);

	particles[i].colorR = clamp(data.startColor.x + data.startColorVar.x*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	particles[i].colorG = clamp(data.startColor.y + data.startColorVar.y*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	particles[i].colorB = clamp(data.startColor.z + data.startColorVar.z*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	particles[i].colorA = clamp(data.startColor.w + data.startColorVar.w*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);

	particles[i].deltaColorR = clamp(data.endColor.x + data.endColorVar.x*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	particles[i].deltaColorG = clamp(data.endColor.y + data.endColorVar.y*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	particles[i].deltaColorB = clamp(data.endColor.z + data.endColorVar.z*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);
	particles[i].deltaColorA = clamp(data.endColor.w + data.endColorVar.w*RANDOM_M11(&RANDSEED), 0.0f, 1.0f);

	particles[i].deltaColorR = (particles[i].deltaColorR - particles[i].colorR) / timeToLive;
	particles[i].deltaColorG = (particles[i].deltaColorG - particles[i].colorG) / timeToLive;
	particles[i].deltaColorB = (particles[i].deltaColorB - particles[i].colorB) / timeToLive;
	particles[i].deltaColorA = (particles[i].deltaColorA - particles[i].colorA) / timeToLive;

	particles[i].size = max(0.0f, data.startSize + data.startSizeVar*RANDOM_M11(&RANDSEED));
	if(data.endSize != -1) {
		float endS = max(0.0f, data.endSize + data.endSizeVar*RANDOM_M11(&RANDSEED));
		particles[i].deltaSize = (endS - particles[i].size) / timeToLive;
	}
	else {
		particles[i].deltaSize = 0.0f;
	}

	particles[i].rotation = data.startSpin + data.startSpinVar*RANDOM_M11(&RANDSEED);
	float endA = data.endSpin + data.endSpinVar*RANDOM_M11(&RANDSEED);
	particles[i].deltaRotation = (endA - particles[i].rotation) / timeToLive;

	vec2 pos;
	pos.x = data.position.x;
	pos.y = data.position.y;
	particles[i].startPosX = pos.x;
	particles[i].startPosY = pos.y;

	if(data.emitterMode == MODE_GRAVITY) {
		particles[i].gravityMode.radialAccel = data.gravityMode.radialAccel + data.gravityMode.radialAccelVar*RANDOM_M11(&RANDSEED);
		particles[i].gravityMode.tangentialAccel = data.gravityMode.tangentialAccel + data.gravityMode.tangentialAccelVar*RANDOM_M11(&RANDSEED);
		float a = radians(data.angle + data.angleVar*RANDOM_M11(&RANDSEED));
		vec2 v;
		v.x = cos(a);
		v.y = sin(a);
		float s = data.gravityMode.speed + data.gravityMode.speedVar*RANDOM_M11(&RANDSEED);
		vec2 dir;
		dir.x = v.x*s;
		dir.y = v.y*s;
		particles[i].gravityMode.dirX = dir.x;
		particles[i].gravityMode.dirY = dir.y;
		if(data.gravityMode.rotationIsDir) {
			particles[i].rotation = -degrees(angle(dir.x, dir.y));
		}
	}
	else {
		particles[i].radiusMode.radius = data.radiusMode.startRadius + data.radiusMode.startRadiusVar*RANDOM_M11(&RANDSEED);
		particles[i].radiusMode.angle = radians(data.angle + data.angleVar*RANDOM_M11(&RANDSEED));
		particles[i].radiusMode.degreesPerSecond = radians(data.radiusMode.rotatePerSecond + data.radiusMode.rotatePerSecondVar*RANDOM_M11(&RANDSEED));
		if(data.radiusMode.endRadius == -1) {
			particles[i].radiusMode.deltaRadius = 0.0f;
		}
		else {
			float endRadius = data.radiusMode.endRadius + data.radiusMode.endRadiusVar*RANDOM_M11(&RANDSEED);
			particles[i].radiusMode.deltaRadius = (endRadius - particles[i].radiusMode.radius) / timeToLive;
		}
	}

	randomSeed[i] = RANDSEED; // update seed
}

void kernel update(global Particle *particles, float dt) {
	particles[get_global_id(0)].timeToLive -= dt;
}

void kernel simulate(global Particle *particles, const ParticleData data, float dt) {
	unsigned int i = get_global_id(0);
	if(particles[i].timeToLive <= 0.0f) return;

	if(data.emitterMode == MODE_GRAVITY) {
		vec2 tmp;
		float tangentialx, tangentialy;
		float radialx = 0.0f, radialy = 0.0f;
		if(particles[i].posx || particles[i].posy) {
			normalize_point(particles[i].posx, particles[i].posy, &radialx, &radialy);
		}
		tangentialx = radialx;
		tangentialy = radialy;
		radialx *= particles[i].gravityMode.radialAccel;
		radialy *= particles[i].gravityMode.radialAccel;

		swap(&tangentialx, &tangentialy);
		tangentialx *= -particles[i].gravityMode.tangentialAccel;
		tangentialy *=  particles[i].gravityMode.tangentialAccel;

		tmp.x = (radialx + tangentialx + data.gravityMode.gravity.x)*dt;
		tmp.y = (radialy + tangentialy + data.gravityMode.gravity.y)*dt;
		particles[i].gravityMode.dirX += tmp.x;
		particles[i].gravityMode.dirY += tmp.y;

		tmp.x = particles[i].gravityMode.dirX * dt * -data.yCoordFlipped;
		tmp.y = particles[i].gravityMode.dirY * dt * -data.yCoordFlipped;
		particles[i].posx += tmp.x;
		particles[i].posy += tmp.y;
	}
	else {
		particles[i].radiusMode.angle += particles[i].radiusMode.degreesPerSecond * dt;
		particles[i].radiusMode.radius += particles[i].radiusMode.deltaRadius * dt;
		particles[i].posx = -cos(particles[i].radiusMode.angle) * particles[i].radiusMode.radius * -data.yCoordFlipped;
		particles[i].posy = -sin(particles[i].radiusMode.angle) * particles[i].radiusMode.radius * -data.yCoordFlipped;
	}
	
	particles[i].x = particles[i].startPosX + particles[i].posx;
	particles[i].y = particles[i].startPosY + particles[i].posy;
	particles[i].colorR += particles[i].deltaColorR * dt;
	particles[i].colorG += particles[i].deltaColorG * dt;
	particles[i].colorB += particles[i].deltaColorB * dt;
	particles[i].colorA += particles[i].deltaColorA * dt;
	particles[i].size += (particles[i].deltaSize * dt);
	particles[i].size = max(0.0f, particles[i].size);
	particles[i].rotation += particles[i].deltaRotation * dt;
	particles[i].rotRad = -radians(particles[i].rotation);
}
