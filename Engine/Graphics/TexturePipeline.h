#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <wrl.h>

#include "IPipeline.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IModel.h"
#include "Mesh.h"

namespace Bat
{
	struct CB_TexturePipelineMatrix
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX viewproj;
	};

	class TexturedModel : public IModel
	{
	public:
		TexturedModel( TexMesh mesh );
		TexturedModel( std::vector<TexMesh> meshes );

		virtual void Draw( IPipeline* pPipeline ) const override;
	private:
		std::vector<TexMesh> m_Meshes;
	};

	class TexturePipelineParameters : public IPipelineParameters
	{
	public:
		TexturePipelineParameters( const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& viewproj, Texture* pTexture )
			:
			pTexture( pTexture )
		{
			transform.world = world;
			transform.viewproj = viewproj;
		}
		CB_TexturePipelineMatrix* GetTransformMatrix()
		{
			return &transform;
		}
		ID3D11ShaderResourceView* GetTextureView() const
		{
			return pTexture->GetTextureView();
		}
	private:
		CB_TexturePipelineMatrix transform;
		Texture* pTexture;
	};

	class TexturePipeline : public IPipeline
	{
	public:
		TexturePipeline( const std::wstring& vsFilename, const std::wstring& psFilename );

		void BindParameters( IPipelineParameters* pParameters ) override;
		void Render( UINT vertexcount ) override;
		void RenderIndexed( UINT indexcount ) override;
	private:
		VertexShader m_VertexShader;
		PixelShader m_PixelShader;
	};
}