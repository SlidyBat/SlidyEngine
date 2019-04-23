#include "CommonPS.hlsli"

Texture2D SceneTexture : register(T_SLOT_0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
    return SceneTexture.Sample(WrapSampler, input.tex);;
}