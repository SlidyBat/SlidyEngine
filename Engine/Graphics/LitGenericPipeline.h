#pragma once

#include "IPipeline.h"
#include "ConstantBuffer.h"
#include "ShaderBuffers.h"
#include "Light.h"
#include "Material.h"

namespace Bat
{
	class LitGenericPipeline : public IPipeline
	{
	public:
		void Render( IGPUContext* pContext,
			const Mesh& mesh,
			const Camera& camera,
			const DirectX::XMMATRIX& world_transform,
			const std::vector<Entity>& light_ents,
			const std::vector<DirectX::XMMATRIX>& light_transforms );
	private:
		std::vector<ShaderMacro> BuildMacrosForMesh( const Mesh& mesh ) const;

		void BindTransforms( IGPUContext* pContext, const Camera& camera, const DirectX::XMMATRIX& world_transform );
		void BindMaterial( IGPUContext* pContext, const Material& material );
		void BindLights( IGPUContext* pContext, const std::vector<Entity>& light_ents, const std::vector<DirectX::XMMATRIX>& light_transforms );
	private:
		struct CB_LitGenericPipelineMatrix
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX viewproj;
		};

		struct CB_LitGenericPipelineMaterial
		{
			ShaderMaterial material;
		};

		struct CB_LitGenericPipelineLights
		{
			uint32_t    num_lights;
			float       _pad0[3];
			ShaderLight lights[MAX_LIGHTS];
		};
	private:
		ConstantBuffer<CB_LitGenericPipelineMatrix>   m_cbufTransform;
		ConstantBuffer<CB_LitGenericPipelineMaterial> m_cbufMaterial;
		ConstantBuffer<CB_LitGenericPipelineLights>    m_cbufLightParams;
	};
}