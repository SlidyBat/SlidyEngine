#include "PCH.h"
#include "ShaderManager.h"

#include "Globals.h"

#include "ConstantBuffer.h"

namespace Bat
{
	std::unordered_map<std::type_index, std::unique_ptr<IPipeline>> ShaderManager::m_mapPipelines;

	struct PSGlobals
	{
		// width/height of screen
		Vec2 resolution;
		float time;
		float deltatime;
		Vec3 camera_pos;
		float pad;
	};

	void ShaderManager::BindShaderGlobals( const Camera* pCamera, const Vec2& resolution, IGPUContext* pContext )
	{
		static ConstantBuffer<PSGlobals> cb_globals;
		static std::vector<std::unique_ptr<ISampler>> samplers;

		static bool initialized = false;
		if( !initialized )
		{
			// Initialize shader globals
			SamplerDesc sampler_desc;
			sampler_desc.filter = SampleFilter::ANISOTROPIC;
			sampler_desc.mip_lod_bias = -0.5f;
			sampler_desc.max_anisotropy = 8;
			sampler_desc.comparison_func = ComparisonFunc::ALWAYS;
			sampler_desc.border_color[0] = 0.0f;
			sampler_desc.border_color[1] = 0.0f;
			sampler_desc.border_color[2] = 0.0f;
			sampler_desc.border_color[3] = 0.0f;
			sampler_desc.min_lod = 0;
			sampler_desc.max_lod = FLT_MAX;

			// wrap sampler
			sampler_desc.address_u = TextureAddressMode::WRAP;
			sampler_desc.address_v = TextureAddressMode::WRAP;
			sampler_desc.address_w = TextureAddressMode::WRAP;
			samplers.emplace_back( gpu->CreateSampler( sampler_desc ) );

			// clamp sampler
			sampler_desc.address_u = TextureAddressMode::CLAMP;
			sampler_desc.address_v = TextureAddressMode::CLAMP;
			sampler_desc.address_w = TextureAddressMode::CLAMP;
			samplers.emplace_back( gpu->CreateSampler( sampler_desc ) );

			// mirror sampler
			sampler_desc.address_u = TextureAddressMode::MIRROR;
			sampler_desc.address_v = TextureAddressMode::MIRROR;
			sampler_desc.address_w = TextureAddressMode::MIRROR;
			samplers.emplace_back( gpu->CreateSampler( sampler_desc ) );

			initialized = true;
		}

		// Bind shader globals
		for( size_t i = 0; i < samplers.size(); i++ )
		{
			gpu->GetContext()->SetSampler( ShaderType::PIXEL, samplers[i].get(), i );
		}

		PSGlobals g;
		g.resolution = resolution;
		g.time = g_pGlobals->elapsed_time;
		g.deltatime = g_pGlobals->deltatime;
		g.camera_pos = pCamera ? pCamera->GetPosition() : Vec3{ 0.0f, 0.0f, 0.0f };
		cb_globals.Update( pContext, g );
		pContext->SetConstantBuffer( ShaderType::PIXEL, cb_globals, PS_CBUF_GLOBALS );
	}
}
