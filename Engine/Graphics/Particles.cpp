#include "PCH.h"
#include "Particles.h"

#include "CoreEntityComponents.h"

namespace Bat
{
	static Particle CreateParticle( const Vec3& pos, const Vec4& colour )
	{
		Particle new_particle;
		new_particle.position = pos;
		new_particle.velocity = { Math::GetRandomFloat( -0.1f, 0.1f ), Math::GetRandomFloat( 0.1f, 0.5f ), Math::GetRandomFloat( -0.1f, 0.1f ) };
		new_particle.age = 0.0f;
		new_particle.colour = colour;
		return new_particle;
	}

	void ParticleSystem::Update( EntityManager& world, float dt )
	{
		for( Entity e : world )
		{
			if( !e.Has<ParticleEmitterComponent>() )
			{
				continue;
			}

			auto& hier = e.Get<HierarchyComponent>();

			DirectX::XMVECTOR vscale, vrot, vpos;
			DirectX::XMMatrixDecompose( &vscale, &vrot, &vpos, hier.abs_transform );
			Vec3 emitter_pos = vpos;

			auto& emitter = e.Get<ParticleEmitterComponent>();
			emitter.particles.resize( MAX_PARTICLES );

			emitter.timer += dt;
			float creation_interval = 1.0f / emitter.particles_per_sec;
			int particles_to_create = 0;
			while( emitter.timer >= creation_interval )
			{
				particles_to_create++;
				emitter.timer -= creation_interval;
			}

			for( int i = 0; i < emitter.num_particles; i++ )
			{
				Particle& p = emitter.particles[i];
				const float gravity = -9.8f;
				p.velocity.y += gravity * dt * emitter.gravity_multiplier;
				p.position += p.velocity * dt;
				p.colour = emitter.gradient.Get( p.age / emitter.lifetime ).AsVector();

				p.age += dt;
				if( p.age >= emitter.lifetime )
				{
					if( particles_to_create )
					{
						p = CreateParticle( emitter_pos, emitter.gradient.Get( 0.0f ).AsVector() );
						particles_to_create--;
					}
					else
					{
						p = emitter.particles[emitter.num_particles - 1];
						emitter.num_particles--;
						i--;
					}
				}
			}

			for( int i = 0; i < particles_to_create; i++ )
			{
				if( emitter.num_particles > MAX_PARTICLES )
				{
					return;
				}

				emitter.particles[emitter.num_particles] = CreateParticle( emitter_pos, emitter.gradient.Get( 0.0f ).AsVector() );
				emitter.num_particles++;
			}
		}
	}
}
