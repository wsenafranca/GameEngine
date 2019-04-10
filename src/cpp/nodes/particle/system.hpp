#ifndef NODES_PARTICLE_SYSTEM_HPP
#define NODES_PARTICLE_SYSTEM_HPP

// based in cocos2dx
// https://github.com/cocos2d/cocos2d-x

#include <nodes/node.hpp>
#include <nodes/particle/emitter.hpp>
#include <nodes/particle/particles.hpp>
#include <graphics/vertex_data.hpp>
#include <graphics/batch.hpp>

namespace nodes
{

namespace particle
{

class system : public node {
public:
	static base::pointer<system> create(const base::pointer<particle::emitter>& emitter);
	static base::pointer<system> create(const base::dictionary& dict);
	virtual ~system();

	virtual void update(float dt) override;
	virtual void render() override;
	virtual void add_particles(uint32_t count);

	const base::pointer<particle::emitter>& emitter() const;
	void emitter(const base::pointer<particle::emitter> &emitter);
	const uint32_t& particle_count() const;

protected:
	system();
	virtual void build(const base::pointer<particle::emitter>& emitter);

	base::pointer<particles> particles;
	graphics::vertex_data::particle *_vertices;
	
	base::pointer<particle::emitter> _emitter;
	uint32_t _particle_count;

	float elapsed;
	float emit_counter;
};

}

}

#endif