#include "ColourPipeline.h"
#include "VertexTypes.h"
#include <fstream>
#include "COMException.h"
#include "IGraphics.h"

namespace Bat
{
	ColouredModel::ColouredModel( ColourMesh mesh )
	{
		m_Meshes.emplace_back( mesh );
	}
	ColouredModel::ColouredModel( std::vector<ColourMesh> meshes )
		:
		m_Meshes( std::move( meshes ) )
	{}

	void ColouredModel::Draw( IPipeline* pPipeline ) const
	{
		auto vp = DirectX::XMMatrixTranspose( g_pGfx->GetCamera()->GetViewMatrix() * g_pGfx->GetCamera()->GetProjectionMatrix() );
		auto w = DirectX::XMMatrixTranspose( GetWorldMatrix() );

		for( const auto& mesh : m_Meshes )
		{
			mesh.Bind();
			ColourPipelineParameters params( w, vp );
			pPipeline->BindParameters( &params );
			pPipeline->RenderIndexed( (UINT)mesh.GetIndexCount() );
		}
	}

	ColourPipeline::ColourPipeline( const std::wstring& vsFilename, const std::wstring& psFilename )
		:
		m_VertexShader( vsFilename, ColourVertex::InputLayout, ColourVertex::Inputs ),
		m_PixelShader( psFilename )
	{
		m_VertexShader.AddConstantBuffer<CB_ColourPipelineMatrix>();
	}

	void ColourPipeline::BindParameters( IPipelineParameters* pParameters )
	{
		auto pColourParameters = static_cast<ColourPipelineParameters*>(pParameters);
		m_VertexShader.Bind();
		m_PixelShader.Bind();
		m_VertexShader.GetConstantBuffer( 0 ).SetData( pColourParameters->GetTransformMatrix() );
	}

	void ColourPipeline::Render( UINT vertexcount )
	{
		auto pDeviceContext = g_pGfx->GetDeviceContext();
		pDeviceContext->Draw( vertexcount, 0 );
	}

	void ColourPipeline::RenderIndexed( UINT indexcount )
	{
		auto pDeviceContext = g_pGfx->GetDeviceContext();
		pDeviceContext->DrawIndexed( indexcount, 0, 0 );
	}
}